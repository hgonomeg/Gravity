//silnik animacji do testów algorytmów obsługujących pary
#include "wuxing.hpp"
#include "tianche.hpp"
#include "sequential.hpp"

void wuxing::draw(sf::RenderTarget& tgt,sf::RenderStates st) const
{	
		for(auto& x: fixed_lines)
		{
			sf::Vertex line[2] = {x.first,x.second};
			tgt.draw(line,2,sf::Lines,st);
		}
		for(auto& x: animated_lines)
		{
			sf::Vertex line[2] = {x.first,x.second};
			tgt.draw(line,2,sf::Lines,st);
		}
		for(auto& x: nodes) 
			tgt.draw(x,st);
}

wuxing::wuxing(int point_count,sf::Vector2u viewport_size)
{
	this->point_count = point_count;
	finished =  false;
	this->viewport_size = viewport_size;
	pair_count = 0;

	layout_nodes();
	animation_thread = nullptr;
}

void wuxing::layout_nodes() {
	std::lock_guard<std::mutex> lock(nodes_mutex);

	auto full_rotation = 2*(float)M_PI;
	float almost_half = 0.45;
	for(int i=0;i<point_count;i++)
	{
		nodes.push_back(node(
							{
								//x:  window center + circle's x coordinate (sinus) in the scale of the window
								viewport_size.x/2.f + ( (float)sin(i/(float)point_count*full_rotation) * viewport_size.x*almost_half ),
								//y: window center (minus 10px due to inscriptions taking up some space) - circle's y coordinate (cosinus) in the scale of the window
								(viewport_size.y/2.f - 10.f) - ((float)cos(i/(float)point_count*full_rotation) * viewport_size.y*almost_half )
							}
							));
	}
}

bool wuxing::quit()
{
	std::lock_guard<std::mutex> lock(finish_mutex);
	{
		return finished;
	}
}

int wuxing::get_pair_count()
{
	return pair_count;
}

std::chrono::milliseconds wuxing::get_best_interval()
{
	auto point_count_squared = point_count*point_count;
	return std::chrono::milliseconds((long long)(750/(float)point_count_squared));
}

void wuxing::animate()
{
	if(animation_thread==nullptr)
	{
		auto animation_thread_func = [this]()
		{
			node_stepper* ns;
			//a two-step for loop might seem stupid but it makes it structurally nicer
			for(int i=0;i<2&&!quit();i++)
			{
				switch(i) //i represents two algorithms that the animation aims to compare
				{		  
					case 0: //sequential; a regular mundane non-parallel stepping
						ns = new sequential_node_stepper(nodes,this);
						break;
					case 1: //tianche: parallelized pair evaluation
						if(quit()) 
							return;
						ns = new tianche(nodes,this);
					break;
				}
				//while(we are still there && the algorihm has not yet finished its' job or the animations aren't finished yet)
				while(!quit() && (animated_lines.size()>0||!ns->finished()))
				{
					nodes_mutex.lock();
					
					for(auto i=animated_lines.begin();i!=animated_lines.end();)
					{
						if(i->tick()) //if the animation of i has been completed
						{
							fixed_lines.push_back(std::pair<sf::Vertex,sf::Vertex>(i->first,i->second)); //it is now a fixed line
							i = animated_lines.erase(i); //and the animated line gets deleted
						}
						else
						{
							i++;
						}
						
					}
					nodes_mutex.unlock();
					//this does not need to run again and again like crazy
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}
				delete ns;
				if(quit())
					return;
				std::this_thread::sleep_for(std::chrono::milliseconds(5000)); //we do not want the algorithms to start immidiately after one another
				//clean up the visual state of wuxing before we start showing next algorithm
				nodes_mutex.lock();
				fixed_lines.clear();
				pair_count = 0;
				nodes_mutex.unlock();
			}
		};
		animation_thread = new std::thread(animation_thread_func);
	}
}

wuxing::~wuxing()
{
	finish_mutex.lock();
	//let other threads know that it is time to go
	finished = true;
	finish_mutex.unlock();
	if(animation_thread)
	{
		if(animation_thread->joinable()) 
			animation_thread->join();
		delete animation_thread;
	}
}

void wuxing::evaluate_pair(const std::list<node>::const_iterator& lhs,const std::list<node>::const_iterator& rhs)
{
	std::lock_guard<std::mutex> lock(nodes_mutex);
	{
		//animate a line between two nodes to indicate that their pair has been evaluated
		animated_lines.push_back(animated_line(lhs->get_location(),rhs->get_location()));
		pair_count++;
	}
}

bool animated_line::tick()
{
	//go towards the desired position
	second.position += delta;
	sf::Vector2f diff = {(desired.x-second.position.x),(desired.y-second.position.y)};
	//when the current position of the second vertex is approximately sqrt(5) away from the desired location, we call it a day
	return sqrt(diff.x*diff.x+diff.y*diff.y)<=5;
}

animated_line::animated_line(const sf::Vector2f& first, const sf::Vector2f& second)
{
	this->first.position = first;
	this->first.color = sf::Color::Green;
	this->second.color = sf::Color::Green;
	desired = second;
	this->second.position = first; //at the beginning of the animation, the second vertex is at the beginning
	delta={(second.x-first.x)/20.f,(second.y-first.y)/20.f};
}

int main()
{
	sf::RenderWindow render_window(sf::VideoMode(700,700),"Wuxing");
	sf::Event ev;
	render_window.setFramerateLimit(60);
	
	std::string m_status_text;
	int point_count = 12; //an arbitrary initial value
	global_wuxing = new wuxing(point_count,render_window.getSize());
	global_font = new sf::Font; 
	global_font->loadFromMemory(arimo.data,arimo.size); 
	sf::Text status_text(std::string(m_status_text),*global_font,12); //bottom status text
	
	auto apply_status_text = [&status_text,&m_status_text,&point_count,&render_window](){
		m_status_text="Use L ad P to change node count. Press X to animate.\nCurrent point count: ";
		m_status_text+=std::to_string(point_count);
		m_status_text+=" Current algorithm's pair count: "+std::to_string(global_wuxing->get_pair_count());
		status_text.setString(m_status_text);
		//center the text and align it to the bottom
		status_text.setPosition(render_window.getSize().x/2.f-status_text.getLocalBounds().width/2.f,render_window.getSize().y-(status_text.getLocalBounds().height+5)); 
	};
	apply_status_text();
	
	while(render_window.isOpen())
	{
			while(render_window.pollEvent(ev))
			{
				switch(ev.type)
				{
					case sf::Event::Closed:
					{
						render_window.close();
					}
					case sf::Event::Resized:
					{
						//ensure that the scale transformation does not get weird
						render_window.setView(sf::View(sf::FloatRect(0,0,ev.size.width,ev.size.height)));
						//update layout of nodes
						global_wuxing->layout_nodes();
						break;
					}
					case sf::Event::KeyPressed:
					{
						switch (ev.key.code)
						{
							case sf::Keyboard::P:
							{
								point_count++;
								delete global_wuxing; 
								global_wuxing = new wuxing(point_count,render_window.getSize());
								apply_status_text();
								break;
							}
							case sf::Keyboard::L:
							{
								if(point_count>3) 
								{
									point_count--;
									delete global_wuxing; 
									global_wuxing = new wuxing(point_count,render_window.getSize());
									apply_status_text();
								}
								break;
							}
							case sf::Keyboard::X:
							{
								global_wuxing->animate();
								break;
							}
						}
						break;
					}
				}
			}
			
			render_window.clear(sf::Color(255,125,125));

			global_wuxing->nodes_mutex.lock();
				apply_status_text();
				render_window.draw(*global_wuxing);
			global_wuxing->nodes_mutex.unlock();


			render_window.draw(status_text);
			render_window.display();
	}
	delete global_wuxing;
	delete global_font;
	return 0;
}