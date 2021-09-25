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
int mixCount = 200;  //셔플



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
	//index와 blank를 swap 
	auto piece = game_board[index];
	game_board[index] = game_board[blank];
	game_board[index]->locate(scene, index_X(index), index_Y(index));
	game_board[blank] = piece;
	game_board[blank]->locate(scene, index_X(blank), index_Y(blank));
	blank = index;
}

bool check_move(int index) {
	//index == blank - 1, +1, -4, +4 하면 근처임
	if (blank % 4 > 0 && index == blank - 1) return true; //첫째열
	if (blank % 4 < 3 && index == blank + 1) return true;//마지막열
	if (blank / 4 > 0 && index == blank - 4) return true;
	if (blank / 4 < 3 && index == blank + 4) return true;

	return false;
}

int random_move() {
	int index = rand() % 16;
	while (!check_move(index)) { //안되면 rand 다시
		index = rand() % 16;
	}
	return index;
}

void start_game() {

	mixCount = 200; //셔플
	timer->set(animationTime);
	timer->start();


	auto timer2 = Timer::create(150.f);
	showTimer(timer2);
	timer2->setOnTimerCallback([&](TimerPtr)->bool {
		showMessage("시간 초과");
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
	showMessage("완성!!");
}

void end_game2() {
	game_board[blank]->show();
	start->show();
	stop->show();
	showMessage("포기ㅠㅜ");
}

void init_game() {

	scene = Scene::create("퍼즐 메인", "Images/배경.png");

	char path[20];

	for (int i = 0; i < 16; i++) {
		sprintf_s(path, "Images/%d.png", i + 1);
		game_board[i] = Object::create(path, scene, index_X(i), index_Y(i));
		game_board[i]->setOnMouseCallback([&](auto piece, auto x, auto y, auto action)->bool {
			// piece로부터 index알아내는 함수
			int index = game_index(piece); //클릭 된 object가 board의 몇번째인지 반환
			if (check_move(index)) {
				game_move(index);

				if (check_end()) {
					showMessage("성공!");
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
		showMessage("시간 초과");
		return true;
		});
	timer2->start();
}
*/
int main() {
	srand((unsigned int)time(NULL)); //시드값에 따라서 다른 랜덤값 -> 같으면 같은거 나옴

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	init_game();

	return 0;
}


