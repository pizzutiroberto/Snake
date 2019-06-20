#pragma once

#include <QWidget>
#include <QTimer>

class Snake : public QWidget
{
	Q_OBJECT

	enum Direction{LEFT, RIGHT, UP, DOWN};
	enum GameState{MENU, RUNNING, PAUSED, GAME_OVER};

	private:

		QTimer _engine;
		int _height, _width;
		std::list<QPoint> _snake;
		QPoint _food;
		Direction _dir;
		GameState _state;
		int _score;

	public:

		Snake(int width = 30, int height = 20);

		virtual void keyPressEvent(QKeyEvent *e);
		virtual void paintEvent(QPaintEvent *e);

		void reset();

	public slots:

		void advance();
};