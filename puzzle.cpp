#include <bangtal>
#define _CRT_SECURE_NO_WARNINGS
using namespace bangtal;
#include <cstdlib>

#include<ctime>



ScenePtr scene;
ObjectPtr game_board[16], game_original[16];
ObjectPtr start;
ObjectPtr stop;
ObjectPtr stop2;

TimerPtr timer;
float animationTime = 0.03f;
int mixCount = 200;  //����



int blank = 0;
int game_index(ObjectPtr piece) {
	for (int i = 0; i < 16; i++)
		if (game_board[i] == piece)	return i;
	return -1;

}

int index_X(int index) {
	return  300 + 150 * (index % 4);
}

int index_Y(int index) {
	return  470 - 150 * (index / 4);
}

void game_move(int index) {
	//index�� blank�� swap 
	auto piece = game_board[index];
	game_board[index] = game_board[blank];
	game_board[index]->locate(scene, index_X(index), index_Y(index));
	game_board[blank] = piece;
	game_board[blank]->locate(scene, index_X(blank), index_Y(blank));
	blank = index;
}

bool check_move(int index) {
	//index == blank - 1, +1, -4, +4 �ϸ� ��ó��
	if (blank % 4 > 0 && index == blank - 1) return true; //ù°��
	if (blank % 4 < 3 && index == blank + 1) return true;//��������
	if (blank / 4 > 0 && index == blank - 4) return true;
	if (blank / 4 < 3 && index == blank + 4) return true;

	return false;
}

int random_move() {
	int index = rand() % 16;
	while (!check_move(index)) { //�ȵǸ� rand �ٽ�
		index = rand() % 16;
	}
	return index;
}

void start_game() {

	mixCount = 200; //����
	timer->set(animationTime);
	timer->start();


	auto timer2 = Timer::create(150.f);
	showTimer(timer2);
	timer2->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("�ð� �ʰ�");
		return true;
		});
	timer2->start();


	blank = 15;
	game_board[blank]->hide();
	start->hide();
	stop->hide();



}

bool check_end() {
	for (int i = 0; i < 16; i++) {
		if (game_board[i] != game_original[i])
			return false;
	}
	return true;
}

void end_game() {
	game_board[blank]->show();
	start->show();
	showMessage("�ϼ�!!");
}

void end_game2() {
	game_board[blank]->show();
	start->show();
	stop->show();
	showMessage("����Ф�");
}

void init_game() {

	scene = Scene::create("���� ����", "Images/���.png");

	char path[20];

	for (int i = 0; i < 16; i++) {
		sprintf_s(path, "Images/%d.png", i + 1);
		game_board[i] = Object::create(path, scene, index_X(i), index_Y(i));
		game_board[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
			// piece�κ��� index�˾Ƴ��� �Լ�
			int index = game_index(piece); //Ŭ�� �� object�� board�� ���°���� ��ȯ
			if (check_move(index)) {
				game_move(index);

				if (check_end()) {
					showMessage("����!");
					end_game();
				}
			}



			return true;
			});
		game_original[i] = game_board[i];

	}

	start = Object::create("Images/start.png", scene, 530, 100);
	stop = Object::create("Images/stop.png", scene, 535, 30);
	start->setScale(0.3f);
	stop->setScale(0.6f);
	start->setOnMouseCallback([&](auto, auto x, auto y, auto action)->bool {
		start_game();
		return true;
		});
	stop->setOnMouseCallback([&](auto, auto x, auto y, auto action)->bool {
		end_game2();
		return true;
		});

	timer = Timer::create(animationTime);
	timer->setOnTimerCallback([&](auto)->bool {
		game_move(random_move());

		mixCount--;
		if (mixCount > 0) {
			timer->set(animationTime);
			timer->start();
		}

		return true;
		});

	startGame(scene);
}
/*
void Timer2() {
	auto timer2 = Timer::create(10.f);
	showTimer(timer2);
	timer2->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("�ð� �ʰ�");
		return true;
		});
	timer2->start();
}
*/
int main() {
	srand((unsigned int)time(NULL)); //�õ尪�� ���� �ٸ� ������ -> ������ ������ ����

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	init_game();

	return 0;
}


