#include<ctime>
#include<cstdlib>
#include<iostream>
#include<sstream>
#include<algorithm>
#include<vector>
using namespace std;

class game;

class board{
public:
	board(int rows, int cols)
	:rows(rows), cols(cols)
	{
		b = (new int*[rows+2]{0})+1;
		
		for(int i=-1; i<=rows; i++){
			b[i] = (new int[cols+2]{0})+1;
			for(int j=-1;j<=cols;j++)
				b[i][j]=0;
		}
		
	}
	
	
	
	board(board const&o)
	:rows(o.rows),cols(o.cols)
	{
		b = (new int*[rows+2]{0})+1;
		for(int i=-1; i<=rows; i++){
			b[i] = (new int[cols+2]{0})+1;
			for(int j=-1;j<=cols;j++)
				b[i][j]=o.b[i][j];
		}
	}
	board& operator=(board other){
		std::swap(other.b, b);
		rows=other.rows;
		cols=other.cols;
		
		return *this;
	}
	
	~board(){
		for(int i=-1;i<=rows;i++)
			delete[] (b[i]-1);
		delete [](b-1);
	}
	int& operator()(int x, int y){
		// if(x<0 || y<0 || x>=b.rows ||y>b.cols)
			// throw {x,y};
		return b[x][y];
	}
	const int& operator()(int x, int y)const{
		// if(x<0 || y<0 || x>=b.rows ||y>b.cols)
			// throw {x,y};
		return b[x][y];
	}
	friend class game;
private:
	int rows, cols;
	int** b;
};

class game{
public:
	game(int rows,int cols):rows(rows),cols(cols), b(rows,cols){}
	
	int& operator()(int x, int y){
		// if(x<0 || y<0 || x>=b.rows ||y>b.cols)
			// throw {x,y};
		return b(x,y);
	}
	const int& operator()(int x, int y)const{
		// if(x<0 || y<0 || x>=b.rows ||y>b.cols)
			// throw {x,y};
		return b(x,y);
	}
	
	bool bound(int x, int y){
		return x>=0 && x<rows && y>=0 && y<cols;
	}
	ostream& printOn(ostream& out)const{
		out<<" | ";
		for(int i=0;i<cols;i++)
			out<<(i%10)<<" ";
		out<<"|\n";
		out<<" | ";
		for(int i=0;i<cols;i++)
			out<<"- ";
		out<<"|\n";
		for(int i=0;i<rows;i++){
			out
				<<i%10
				<<"| ";
			for(int j=0;j<cols;j++){
				out<<(b(i,j)?'X':' ')<<' ';
			}
			out<<"|\n";
		}
		out<<" | ";
		for(int i=0;i<cols;i++)
			out<<"- ";
		out<<"|\n";
		return out;
	}
	
	void randomize(){
		for(int i=0;i<rows;i++)
			for(int j=0;j<cols;j++)
				b(i,j)=rand()%2;
	}
	
	void advance(){
		board temp(rows,cols);
		for(int x=-1;x<=1;x++)
			for(int y=-1;y<=1;y++){
				if(x==0 && y==0)
					continue;
				for(int i=0;i<rows;i++){
					for(int j=0;j<cols;j++){
						temp(i,j)+=b(i+x,j+y);
					}
				}
			}
		
		for(int i=0;i<rows;i++){
			for(int j=0;j<cols;j++){
				if(temp(i,j)==3){
					b(i,j) = 1;
				}else if(temp(i,j)==2);
				else b(i,j)=0;
			}
		}
	}
	
private:
	int rows, cols;
	board b;
	
};

ostream& operator<<(ostream& out,game const& g){
	ostringstream oss;
	g.printOn(oss);
	
	out<<oss.str();
	
	return out;
}

string const menu = "Press enter to advance, or:\n"
		"`add X Y` to add something to the cell\n"
		"`del X Y` to delete\n"
		"`tog X Y` to toggle\n"
		"`randomize` to do something\n"
		;

int main(){
	srand(time(NULL));

	cout<<"Enter rowsize and colsize\n";
	
	size_t rz,cz;
	cin>>rz>>cz;
	
	game life(rz,cz);
	
	string input;
	
	cout<<menu;
	while(getline(cin,input)){
		if(input.size()!=0){
			istringstream iss(input);
			string cmd;
			int x, y;
			iss>>cmd>>x>>y;
			
			if(!life.bound(x,y)){
				cout<<"Invalid cell\n\n";
			}
			else if(cmd == "add"){
				life(x,y)=1;
			}else if(cmd=="del"){
				life(x,y)=0;
			}else if(cmd=="tog"){
				life(x,y) = 1-life(x,y);
			}else if(cmd=="randomize")
				life.randomize();
			else cout<<"Invalid command\n\n";
			
		}
		else life.advance();
		
		cout<<life<<endl;
	}
	
	return 0;
}
