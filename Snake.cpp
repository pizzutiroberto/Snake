#include "Snake.h"
#include <QKeyEvent>
#include <QPainter>
#include <iterator>
#include <algorithm>
#include <QCoreApplication>
#include <QImage>
#include <string> 

Snake::Snake(int width, int height) : QWidget(0)
{
	_height = height;
	_width = width;
	connect(&_engine, SIGNAL(timeout()), this, SLOT(advance()));
	reset();
	setFixedSize(width*20, height*20);
}

void Snake::reset()
{
	_snake.clear();
	for(int l=0; l<5; l++)
		_snake.push_front(QPoint(_width/2-5+l, _height/2));
	_dir = RIGHT;
	_food.setX(rand()%_width);
	_food.setY(rand()%_height);
	_score = 0;
	_state = MENU;
	_engine.setInterval(100);
	update();
}

void Snake::keyPressEvent(QKeyEvent *e)
{
	if ((e->key() == Qt::Key_N && _state == MENU) || (e->key() == Qt::Key_P && _state == PAUSED))
	{
		_state = RUNNING;
		_engine.start();
	}
	else if (e->key() == Qt::Key_P && _state == RUNNING)
		_state = PAUSED;
	else if ((e->key() == Qt::Key_Escape && _state == PAUSED) || (e->key() == Qt::Key_Escape && _state == GAME_OVER))
	{
		_state = MENU;
		reset();
	}
	else if (e->key() == Qt::Key_Escape && _state == MENU)
		QCoreApplication::quit();
	else if((e->key() == Qt::Key_W || e->key() == Qt::Key_Up) && _dir != DOWN)
		_dir = UP;
	else if((e->key() == Qt::Key_S || e->key() == Qt::Key_Down) && _dir != UP)
		_dir = DOWN;
	else if((e->key() == Qt::Key_D || e->key() == Qt::Key_Right) && _dir != LEFT)
		_dir = RIGHT;
	else if((e->key() == Qt::Key_A || e->key() == Qt::Key_Left) && _dir != RIGHT)
		_dir = LEFT;
}

void Snake::advance()
{
	if(_dir == RIGHT)
		_snake.push_front(_snake.front()+QPoint(1, 0));
	else if(_dir == LEFT)
		_snake.push_front(_snake.front()+QPoint(-1,0));
	else if(_dir == UP)
		_snake.push_front(_snake.front()+QPoint(0,-1));
	else if(_dir == DOWN)
		_snake.push_front(_snake.front()+QPoint(0, 1));

	auto second = _snake.begin();
	second++;
	if (std::find(second, _snake.end(), _snake.front()) != _snake.end())
	{
		_state = GAME_OVER;
		_engine.stop();
	}
    
	if(_snake.front().x() < 0)
		_snake.front().setX(_snake.front().x() + _width);
	else if(_snake.front().y() < 0)
		_snake.front().setY(_snake.front().y() + _height);
	else if(_snake.front().x() >= _width) 
		_snake.front().setX(_snake.front().x() % _width);
	else if(_snake.front().y() >= _height)
		_snake.front().setY(_snake.front().y() % _height);

	if(_snake.front() == _food)
	{
		_food.setX(rand()%_width);
		_food.setY(rand()%_height);
		_engine.setInterval(_engine.interval()*0.97);
		_score += 10;
	}
	else 
		_snake.pop_back();

	update();
}

void Snake::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.setWindow(0, 0, _width, _height);

	if(_state == MENU)
		painter.drawImage(QRect(0,0,_width,_height), QImage(":img/title-01.png"));
	else if(_state == RUNNING || _state == PAUSED || _state == GAME_OVER)
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(QBrush(QColor(10,10,10)));
		painter.drawRect(painter.window());
		painter.setBrush(QBrush(QColor(99, 167, 69)));
		for(auto & p : _snake)
			painter.drawRect(p.x(), p.y(), 1, 1);
		painter.setPen(QPen(QBrush(QColor(99, 167, 69)), 0.2));
		painter.setBrush(QBrush(QColor(10, 10, 10)));
		painter.drawRoundedRect(_food.x(), _food.y(), 1, 1, 0.3, 0.3);
		QString bottom_text = " P - Pause    |    Score: ";
		bottom_text.append(QString::number(_score));
		painter.setPen(QPen(QBrush(QColor(255, 255, 255)), 0.5));
		painter.setFont(QFont("OCR A Std", 1));
		painter.drawText(QPoint(0, _height), bottom_text);
		if(_state == PAUSED)
		{
			painter.drawImage(QRect(0, 0, _width, _height), QImage(":img/paused-01.png"));
			_engine.stop();
		}
		if(_state == GAME_OVER)
			painter.drawImage(QRect(0, 0, _width, _height), QImage(":img/game_over-01.png"));
	}
}
