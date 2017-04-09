#ifndef OBJECT_H
#define OBJECT_H

#include <string>

using namespace std;

class Object
{
	public:
		Object(string name, int x=0, int y=0){
			this->name = name;
			this->x = x;
			this->y = y;
		}
		virtual ~Object(){}
		void fillNumeriqueValue(int pos[]){
			if (pos){
				x = pos[0];
				y = pos[1];	
			}
		}
		int pos_x(){ return x; }
		int pos_y(){ return y; }
		string getName(){ return name; }
		void setName(string n){ name = n; }

	protected:
		int x, y;
		string name;

	private:
};

#endif // OBJECT_H