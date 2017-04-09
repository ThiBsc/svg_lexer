#ifndef SHAPE_H
#define SHAPE_H

#include <sstream>
#include <map>
#include <utility>
#include "object.h"

class Shape : public Object
{
	public:
		Shape(string name, int x, int y, int w, int h):Object(name, x, y){
			width = w;
			height = h;
		}
		virtual ~Shape(){
			attributs.clear();
			coord.clear();
		}
		int getWidth(){ return width; }
		int getHeight(){ return height; }
		void addAttributs(map<string, string> attrs){
			for(auto p : attrs){
				if (name == "text" && p.first == "stroke")
					attributs.insert(pair<string, string>("fill", p.second));
				else
					attributs.insert(p);
			}
		}
		void modAttribut(string k, string v){
			if (name == "text" && k == "stroke")
				k = "fill";
			try {
				attributs.at(k) = v;
		    } catch (const std::out_of_range &e){
		    	/* Si on est ici, l'attribut n'existe pas
		    	on vérifie que ce n'est pas non plus
		    	les coordonnées de la forme */
		    	try {
		    		*coord.at(k) = atoi(v.c_str());
		    	} catch (const std::out_of_range &e){
		    		/* Ici, ce n'est ni un attribut existant
		    		ni une des coordonnées, donc on insert */
					attributs.insert(pair<string, string>(k, v));
		    	}
		    }
		}
		string getAttributValue(string k){
			string ret;
			try {
				ret = attributs.at(k);
		    } catch (const std::out_of_range &e){
		    	/* Meme principe que pour le modAttribut */
		    	try {
		    		std::ostringstream s;
		    		s << coord.at(k);
		    		ret = s.str();
		    	} catch (const std::out_of_range &e){
		    		ret = "-1";
		    	}
		    }
		    return ret;
		}
		friend bool operator==(const Shape& s1, const Shape& s2){
			bool ret;
			if (s1.name != s2.name)
				ret = false;
			else
				ret = (s1.width*s1.height) == (s2.width*s2.height);
			return ret;
		}
		friend bool operator<(const Shape& s1, const Shape& s2){
			return (s1.width*s1.height) < (s2.width*s2.height);
		}
		friend bool operator>(const Shape& s1, const Shape& s2){
			return (s1.width*s1.height) > (s2.width*s2.height);
		}
		virtual string toString() = 0;

	protected:
		string formatAttributs(){
			string transform = "transform=\"";
			std::ostringstream s;
			for(auto p : attributs){
				if (p.first == "fill" || p.first == "color" || p.first == "stroke")
					s << p.first << "=\"" << p.second << "\" ";
				else{
					if (p.first == "rotate"){
						std::ostringstream s_rotate;
						s_rotate << p.first << "(" << p.second << " " << width/2 << " " << height/2 << ")";
						transform += s_rotate.str();
					}
					else
						transform += p.first+"("+p.second+") ";
				}
			}
			if (transform != "transform=\""){
				transform += "\" ";
				s << transform;
			}
			return s.str();
		}
		int width, height;
		map<string, string> attributs;
		map<string, int*> coord;

	private:
};

class Rect : public Shape
{
	/* <rect width="100" height="80" x="0" y="70" fill="green" /> */
	public:
		Rect(int x, int y, int w, int h):Shape("rect", x, y, w, h){
			coord.insert(pair<string, int*>("x", &this->x));
			coord.insert(pair<string, int*>("y", &this->y));
			coord.insert(pair<string, int*>("width", &this->width));
			coord.insert(pair<string, int*>("height", &this->height));
		}
		virtual string toString(){
			std::ostringstream s;
  			s << "<rect x=\"" << pos_x() << "\" ";
  			s << "y=\"" << pos_y() << "\" ";
  			s << "width=\"" << width << "\" ";
  			s << "height=\"" << height << "\" " << formatAttributs() << "/>";
			return s.str();
		}
};

class Line : public Shape
{
	/* <line x1="5" y1="5" x2="250" y2="95" stroke="red" /> */
	public:
		Line(int x1, int y1, int x2, int y2):Shape("line", x1, y1, x2, y2){
			coord.insert(pair<string, int*>("x1", &this->x));
			coord.insert(pair<string, int*>("y1", &this->y));
			coord.insert(pair<string, int*>("x2", &this->width));
			coord.insert(pair<string, int*>("y2", &this->height));
		}
		virtual string toString(){
			std::ostringstream s;
  			s << "<line x1=\"" << pos_x() << "\" ";
  			s << "y1=\"" << pos_y() << "\" ";
  			s << "x2=\"" << width << "\" ";
  			s << "y2=\"" << height << "\" " << formatAttributs() << "/>";
			return s.str();
		}
};

class Circle : public Shape
{
	/* <circle cx="90" cy="80" r="50" fill="blue" /> */
	public:
		Circle(int cx, int cy, int r):Shape("circle", cx, cy, r, 0){
			coord.insert(pair<string, int*>("cx", &this->x));
			coord.insert(pair<string, int*>("cy", &this->y));
			coord.insert(pair<string, int*>("rayon", &this->width));
		}
		virtual string toString(){
			std::ostringstream s;
  			s << "<circle cx=\"" << pos_x() << "\" ";
  			s << "cy=\"" << pos_y() << "\" ";
  			s << "r=\"" << width << "\" " << formatAttributs() << "/>";
			return s.str();
		}
};

class Text : public Shape
{
	/* <text x="180" y="60">Un texte</text> */
	public:
		Text(int x, int y, string txt):Shape("text", x, y, 0, 0){
			this->txt = txt;
			coord.insert(pair<string, int*>("x", &this->x));
			coord.insert(pair<string, int*>("y", &this->y));
		}
		virtual string toString(){
			std::ostringstream s;
  			s << "<text x=\"" << pos_x() << "\" ";
  			s << "y=\"" << pos_y() << "\" " << formatAttributs() << ">";
  			s << txt << "</text>";
			return s.str();
		}
	private:
		string txt;
};

#endif // SHAPE_H