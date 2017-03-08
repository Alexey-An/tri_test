#include <stdio.h>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

//Вначале программа генерирует координаты вершин треугольника и проверяет их на соответствие условиям задачи.
//Затем программа генерирует массив элементов типа int размером 500 х 500, присваивая им значение 0. 
//Данный массив представляет собой множество пикселей будущего изображения в формате pgm.
//Далее по алгоритму Ву отрисовываются стороны будущего треугольника: элементам массива,
//соответствующим пикселям изображения, присваиваются значения.
//Затем генератор шума создает помехи, искажающие изображение, случайным образом присваивая каждому
//элементу массива значение от 0 до 255. В конце массив записывается в файл с расширением pgm -- будущее изображение.
//После чего программа завершает работу.


//Раздел, где описываются функции для алгоритма Ву
int ipart (float x)
{
    return (int)x;
}

float fpart (float x)
{
    return x - ipart(x);
}

float rfpart (float x)
{
    return 1 - fpart(x);
}

int round (float x)
{
    return ipart (x +0.5);
}

float max (float a, float b, float c)
{
	float max;
	if (a > b) max = a;
	else {
		max = b;
		if (max < c) max = c;
	}
	return max;
}

void swap (float &x, float &y)
{
    float t = x;
    x = y;
    y = t;
}

float abs (float a)
{
	if (a >= 0) return a;
	else return -a;
}

float sq (float x)
{
	return (x * x);
}

//Алгоритм Ву
void wu (float x1, float y1, float x2, float y2, int ham [500][500])
{	
	bool steep;
	float abs1, abs2;
	if (y2 < y1) abs1 = (y1 - y2);
	else abs1 = (y2 - y1);
	if (x2 < x1) abs2 = (x1 - x2);
	else abs2 = (x2 - x1);
	if (abs1 > abs2) steep = true;
	else steep = false;
		
	if (steep == true) {
		swap(x1, y1);
		swap(x2, y2);
	}
	if (x1 > x2) {
        swap (x1, x2);
        swap (y1, y2);
    }
    float dx = x2 - x1;
    float dy = y2 - y1;
    float grad = dy / dx;
    if (dx == 0) grad = 1;
       
    //Обработать начальную точку
    int xend = round (x1);
    float yend = y1 + grad * (xend - x1);
    float xgap = rfpart((x1 + 0.5));
    int xpxl1 = xend;
    int ypxl1 = ipart (yend);
    if (steep == true) {
    	ham [ypxl1][xpxl1] = 255 * rfpart(yend) * xgap;
    	ham [ypxl1+1][xpxl1] = 255 * fpart(yend) * xgap;
    }
    else {
    	ham [xpxl1][ypxl1] = 255 * rfpart(yend) * xgap;
    	ham [xpxl1][ypxl1+1] = 255 * fpart(yend) * xgap;
    }
    float intery = yend + grad;
    
    //Обработать конечную точку
    xend = round (x2);
    yend = y2 + grad * (xend - x2);
    xgap = fpart (x2 + 0.5);
    int xpxl2 = xend;
    int ypxl2 = ipart(yend);
    if (steep == true) {
		ham [ypxl2][xpxl2] = 255 * rfpart(yend) * xgap;
		ham [ypxl2+1][xpxl2] = 255 * fpart(yend) * xgap;
	}
	else {
	    ham [xpxl2][ypxl2] = 255 * rfpart(yend) * xgap;
	    ham [xpxl2][ypxl2+1] = 255 * fpart(yend) * xgap;
	}
    
    //Основной цикл
    if (steep == true) {
        for (int i = xpxl1 + 1; i < xpxl2; i++) {
        	ham [ipart(intery)][i] = 255 * rfpart(intery);
        	ham [ipart(intery) + 1][i] = 255 * fpart(intery);
        	intery = intery + grad;
        }
    }
    else {
	    for (int i = xpxl1 + 1; i < xpxl2; i++) {
        	ham [i][ipart(intery)] = 255 * rfpart(intery);
        	ham [i][ipart(intery) + 1] = 255 * fpart(intery);
        	intery = intery + grad;
        }
    }
}

int main (int argc, char *argv[]) {   
    int flag, g = 0;
    
    //Генератор координат вершин треугольника.
    float x0, y0, x1, y1, x2, y2, a, b, c, big, cosb, cosa, cosc, cosbig, cosm1, cosm2;
    
	do {    	   
		srand(time(0));
		x0 = (rand() %500 + (rand() %99)*0.01); 							//A (x0-x1) (y0-y1)
		y0 = (rand() %500 + (rand() %99)*0.01);								//A (x2-x1) (y2-y1)
		x1 = (rand() %500 + (rand() %99)*0.01);								//B
		y1 = (rand() %500 + (rand() %99)*0.01);								//B
		x2 = (rand() %500 + (rand() %99)*0.01);								//C
		y2 = (rand() %500 + (rand() %99)*0.01);								//C
		a = sqrt(sq(x0 - x1) + sq(y0 - y1));
		b = sqrt(sq(x1 - x2) + sq(y1 - y2));
		c = sqrt(sq(x2 - x0) + sq(y2 - y0));
		big = max(a, b, c);
		//cosa = (((x0-x1)*(y2-y1) + (y0-y1)*(x2-x1))/(a*b)); // a b
		//cosb = (((x1-x0)*(y2-y0) + (y1-y0)*(x2-x0))/(a*c));
		//cosc = (((x0-x2)*(y1-y2) + (y0-y2)*(x1-x2))/(c*b));
		cosa = ((sq(c)+sq(b)-sq(a))/(2*c*b));
		cosb = ((sq(a)+sq(c)-sq(b))/(2*c*a));
		cosc = ((sq(a)+sq(b)-sq(c))/(2*a*b));
		if (big == a) {
			cosbig = cosa;					//cosbig = (((x0 - x2) * (y1 - y2) + (y0 - y2) * (x1 - x2)) / (b * c));				//cosa
			cosm1 = cosc;					//cosmin1 = (((x1 - x0) * (y2 - y0) + (y1 - y0) * (x2 - x0)) / (c * a));			//cosb 
			cosm2 = cosb;					//cosmin2 = (((x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1)) / (a * b));			//cosc
			flag = 1;
		}
		else {
			if (big == c) {
				cosbig = cosc;				//cosbig = (((x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1)) / (a * b));				//cosc
				cosm1 = cosa;				//cosmin1 = (((x0 - x2) * (y1 - y2) + (y0 - y2) * (x1 - x2)) / (b * c));			//cosa
				cosm2 = cosb;				//cosmin2 = (((x1 - x0) * (y2 - y0) + (y1 - y0) * (x2 - x0)) / (c * a)); 			//cosb
				flag = 3;
			}
			else {
				cosbig = cosb;				//cosbig = (((x1 - x0) * (y2 - y0) + (y1 - y0) * (x2 - x0)) / (c * a));				//cosb
				cosm1 = cosa;				//cosmin1 = (((x0 - x2) * (y1 - y2) + (y0 - y2) * (x1 - x2)) / (b * c));			//cosa
				cosm2 = cosc;				//cosmin2 = (((x0 - x1) * (y2 - y1) + (y0 - y1) * (x2 - x1)) / (a * b));			//cosc
				flag = 2;
			}			
		}
		g++;
	}
	while ((a < 100) || (b < 100) || (c < 100) || (cosm1 >= 0.866025404) || (cosm2 >= 0.866025404)); 
	
    //Заполнение массива нулями
    ofstream fout("image.pgm");
    fout << "P2" << endl;
    fout << "# THIS FILE IS A PGM IMAGE" << endl;
    fout << "500 500" << endl;
    fout << "255" << endl;
    int i, j, im [500][500];
    for (i = 0; i <= 499; i++) {
        for (j = 0; j <= 499; j++) {
            im [j][i] = 0;
        }
    }
    
    //Отрисовка треугольника    
    wu (x0, y0, x1, y1, im);
    wu (x1, y1, x2, y2, im);
    wu (x0, y0, x2, y2, im);
    
    //Генератор шума
    float p;
    p = atof (argv[1]);
    srand(time(0));
    for (i = 0; i != 499; i++) {
        for (j = 0; j != 499; j++) {
            if ((p != 0) && (p * 10) > (rand() %50)) im[i][j] = (rand() %255);
        }
    }
    
    //Запись масива в файл
    for (i = 0; i <= 499; i++) {
        for (j = 0; j <= 499; j++) {
            fout << im [j][i] << "   ";
        }
        fout << endl;
    }
    fout.close();
    cout << x0 << "  " << y0 << endl;
    cout << x1 << "  " << y1 << endl;
    cout << x2 << "  " << y2 << endl;
    cout << a << endl;
    cout << b << endl;
    cout << c << endl;
    cout << cosa << endl;
    cout << cosb << endl;
    cout << cosc << endl;
    cout << flag << endl;
    cout << g << endl;
    return 0;
}
