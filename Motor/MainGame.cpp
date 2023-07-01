#include "MainGame.h"
#include <iostream>
#include "Error.h"
#include <random>
#include <ctime>

using namespace std;

MainGame::MainGame() {
	width = 800;
	height = 600;
	gameState = GameState::PLAY;	
	camera2D.init(width, height);
	ShowData = false;
	totalHumans = humans.size();
	totalZombies = zombies.size();

	activeChangeLevel=false;
	curLevel=0;

}

MainGame::~MainGame() {

}
void MainGame::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
			case SDL_QUIT:
				gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				/*cout << "Posicion del mousec " << event.motion.x << " " << event.motion.y << endl;
				inputManager.setMouseCoords(event.motion.x, event.motion.y);
				glm::vec2 mouseCoords = camera2D.convertToScreenWorld(inputManager.getMouseCoords());
				//cout << "Nueva posicion de acuerdo a camara " <<  mouseCoords.x
					//	<< " " << mouseCoords.y << endl;*/
				break;
			case SDL_KEYUP:
				inputManager.releaseKey(event.key.keysym.sym);
				break;
			case SDL_KEYDOWN:
				inputManager.pressKey(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				inputManager.pressKey(event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				inputManager.releaseKey(event.button.button);
				break;
		}
		handleInput();
	}
	
}

void MainGame::handleInput()
{
	const float SCALE_SPEED = 0.1f;
	if (inputManager.isKeyPressed(SDLK_q)) {
		camera2D.setScale(camera2D.getScale() + SCALE_SPEED);
	}

	if (inputManager.isKeyPressed(SDLK_e)) {
		camera2D.setScale(camera2D.getScale() - SCALE_SPEED);
	}
	if (inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		//cout << "CLICK IZQUIERDO" << endl;
		createBullet();
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {
		//cout << "CLICK DERECHo" << endl;
	}

	if (inputManager.isKeyPressed(SDL_BUTTON_MIDDLE)) {
		//cout << "CLICK CENTRO" << endl;
	}
	if (inputManager.isKeyPressed(SDLK_f)) {
		/*if (this->ShowData) {
			this->ShowData = false;
		}
		else {*/
			this->ShowData = true;
			system("cls");
			cout << "DATOS :_________________________" << endl;
			cout << "cantidad de zombies : " << zombies.size() << endl;
			cout << "cantidad de humanos  : " << humans.size() << endl;
		//}
		
	}

	if (inputManager.isKeyPressed(SDLK_n)){
		this->activeChangeLevel = true;
		system("cls");
		cout << "ingresa el level que quiere establecer";
	}
	if (inputManager.isKeyPressed(SDLK_1)) {
		if(this->activeChangeLevel){
			currentLevel = 0;
			curLevel = 0;
			vector<Human*> humansv1;
			vector<Zombie*> zombiesv1;
			this->humans = humansv1;
			this->zombies = zombiesv1;
			initLevel();
		}
	}
	if (inputManager.isKeyPressed(SDLK_2)) {
		if (this->activeChangeLevel) {
			currentLevel = 1;
			curLevel = 1;

			vector<Human*> humansv2;
			vector<Zombie*> zombiesv2;

			this->humans = humansv2;
			this->zombies = zombiesv2;

			initLevel();
		}
	}
	if (inputManager.isKeyPressed(SDLK_3)) {
		if (this->activeChangeLevel) {
			currentLevel = 2;
			curLevel = 2;

			vector<Human*> humansv3;
			vector<Zombie*> zombiesv3;
			this->humans = humansv3;
			this->zombies = zombiesv3;
			initLevel();
		}
	}



}

void MainGame::createBullet() {
	glm::vec2 mouseCoords = 
			camera2D.convertToScreenWorld(inputManager.getMouseCoords());
	glm::vec2 playerPosition(0, 0);
	glm::vec2 direction = mouseCoords - player->getPosition();
	direction = glm::normalize(direction);
	//bullets.emplace_back(playerPosition, direction, 1.0f, 1000);
	
	//bullets.push_back(new Bullet(playerPosition, direction, 1.0f, 1000));
	Bullet* bullet = new Bullet(player->getPosition(), direction, 1.0f, 1000);
	bullets.push_back(bullet);
}

void MainGame::initShaders()
{
	program.compileShaders("Shaders/colorShaderVert.txt", "Shaders/colorShaderFrag.txt");
	program.addAtribute("vertexPosition");
	program.addAtribute("vertexColor");
	program.addAtribute("vertexUV");
	program.linkShader();
}

void MainGame::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window.create("Mundo 1", width, height,0);
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Glew not initialized");
	}
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	initLevel();
	initShaders();
}

void MainGame::initLevel() {
	levels.push_back(new Level("Level/level1.txt"));
	levels.push_back(new Level("Level/level2.txt"));
	levels.push_back(new Level("Level/level3.txt"));
	currentLevel =curLevel;
	//inicializar humans,player y zombie
	player = new Player();
	player->init(5.0f, levels[currentLevel]->getPlayerPosition(), &inputManager);
	spriteBatch.init();
	hudBatch.init();

	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int>randPosX(
		1, levels[currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int>randPosY(
		1, levels[currentLevel]->getHeight() - 2);

	for (size_t i = 0; i < levels[currentLevel]->getNumHumans(); i++)
	{
		humans.push_back(new Human());
		glm::vec2 pos(randPosX(randomEngine) * TILE_WIDTH,
			randPosY(randomEngine) * TILE_WIDTH);
		humans.back()->init(1.0f, pos);
	}

	const std::vector<glm::vec2>& zombiePosition =
		levels[currentLevel]->getZombiesPosition();

	for (size_t i = 0; i < zombiePosition.size(); i++)
	{
		zombies.push_back(new Zombie());
		zombies.back()->init(3.9f, zombiePosition[i]);
	}
	spriteFont = new SpriteFont("Fonts/arial.ttf",64);

}

void MainGame::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();
	glActiveTexture(GL_TEXTURE0);

	glm::mat4 cameraMatrix = camera2D.getCameraMatrix();
	GLuint pCameraLocation = program.getUniformLocation("pCamera");
	glUniformMatrix4fv(pCameraLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	GLuint imageLocation = program.getUniformLocation("myImage");
	glUniform1i(imageLocation, 0);
	spriteBatch.begin();
	levels[currentLevel]->draw();
	player->draw(spriteBatch);
	for (size_t i = 0; i < humans.size(); i++)
	{
		humans[i]->draw(spriteBatch);
	}
	for (size_t i = 0; i < bullets.size(); i++)
	{
		bullets[i]->draw(spriteBatch);
	}
	for (size_t i = 0; i < zombies.size(); i++)
	{
		zombies[i]->draw(spriteBatch);
	}
	spriteBatch.end();
	spriteBatch.renderBatch();
	drawHud();
	glBindTexture(GL_TEXTURE_2D, 0);
	program.unuse();
	window.swapWindow();
	
	if (this->humans.size()<50&&currentLevel<2) {
		this->currentLevel += 1;
		this->curLevel += 1;
		vector<Human*> humansv4;
		vector<Zombie*> zombiesv4;
		this->humans = humansv4;
		this->zombies = zombiesv4;
		initLevel();
	}

}

void MainGame::drawHud()
{
	glm::mat4 cameraMatrix = camera2D.getCameraMatrix();
	GLuint pCameraLocation = program.getUniformLocation("pCamera");
	glUniformMatrix4fv(pCameraLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	char buffer[256];
	hudBatch.begin();
	sprintf_s(buffer, "HOLAAAAAAA");
	Color color;
	color.set(255, 255, 255, 255);
	spriteFont->draw(hudBatch, buffer, glm::vec2(0, 0),
		glm::vec2(0.5), 0.0f, Color(255,255,255,255));
	hudBatch.end();
	hudBatch.renderBatch();
}

void MainGame::run() {
	init();
	update();
}

void MainGame::updateElements() {
	player->update(levels[currentLevel]->getLevelData(), humans, zombies);
	for (size_t i = 0; i < humans.size(); i++)
	{
		humans[i]->update(levels[currentLevel]->getLevelData(),humans,zombies);
	}
	for (size_t i = 0; i < zombies.size(); i++) {
		zombies[i]->update(levels[currentLevel]->getLevelData(), humans, zombies);

		for (size_t j = 0; j < humans.size(); j++)
		{
			if (zombies[i]->collideWithAgent(humans[j])) {
				zombies.push_back(new Zombie());
				zombies.back()->init(1.3f, humans[j]->getPosition());
				delete humans[j];
				humans[j] = humans.back();
				humans.pop_back();
				if(this->ShowData){
					this->totalHumans = humans.size();
					this->totalZombies = zombies.size();
					system("cls");
					cout << "DATOS : ____________________" << endl;
					cout << "cantidad de zombies : " << this->totalZombies << endl;
					cout << "cantidad de humanos  : " << this->totalHumans << endl;
				}
				


			}
		}
	}
	for (size_t i = 0; i < bullets.size();)
	{
		if (bullets[i]->update()) {
			bullets[i] = bullets.back();
			bullets.pop_back();


		}
		else {
			i++;
		}
	}
	

}

void MainGame::update() {
	while (gameState != GameState::EXIT) {
		draw();
		camera2D.update();
		camera2D.setPosition(player->getPosition());
		processInput();
		updateElements();
		

	}
}