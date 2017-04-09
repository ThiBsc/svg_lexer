#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <string.h>
#include <map>
#include <iostream>
#include <vector>
#include <iterator>

#include "function_mapper.h"
#include "shape.h"

using namespace std;

/* Post declaration */
class BaseCommand;
template <class... ArgTypes> class Command;
class CommandManager;
class Object;
class Shape;

/* Declaration */
struct dimension
{
  int w;
  int h;
};
typedef struct dimension Dimension;

struct infos
{
	char* title;
	char* desc;
};
typedef struct infos Infos;

void canvas(Dimension area);
void node(const char *node, const char* content);
void add_attr(string k, string v);
void write_file();
void free_memory();

/* Variables */
FILE *f;
CommandManager f_map; 	// Mapper de fonction
Dimension canevas;		// Structure canvas
Infos infos;			// Structure info
vector<Shape*> shapeList;	// liste de forme
map<string, string> attrs;	// liste des attributs de la forme
map<string, Shape*> variables; // dictionnaire des variables Shape

/* Definition */
void canvas(Dimension area){
  	fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"%d\" height=\"%d\">\n", area.w, area.h);
}

void node(const char *node, const char* content){
	fprintf(f, "<%s>%s</%s>\n", node, content, node);
}

void write_file(){
	f = fopen("render.svg", "w");
	if (f != NULL){
		/* Write file */
		fprintf(f, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		f_map.execute("canvas", canevas);
		f_map.execute("node", "title", (const char*)infos.title);
		f_map.execute("node", "desc", (const char*)infos.desc);
		for(auto s : shapeList){
		  fprintf(f, "%s\n", s->toString().c_str());
		}
		fprintf(f, "</svg>\n");
		/* Close */
		fclose(f);
	}
	else
		cout << "Unable to write file." << endl;
}

void free_memory(){
	for(auto s : shapeList)
		delete s;
	attrs.clear();
	shapeList.clear();
}

void add_attr(string k, string v){
	string val = v;
	try {
		attrs.at(k) = val;
	} catch (const std::out_of_range &e){
		attrs.insert(pair<string, string>(k, val));
	}
}

#endif // UTILS_H