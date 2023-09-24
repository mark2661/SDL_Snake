#include "Game.hpp"
#include "utils.hpp"


Game::Game(){}

Game::Game(const int screenWidth, const int screenHeight, const int gridSpacing)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->snake = Snake(this->screenWidth/2, this->screenHeight/2, this->gridSpacing, this->gridSpacing);
}

void Game::populateGrid()
{
    const int num_grid_rows = this->screenWidth / this->gridSpacing;
    const int num_grid_cols = this->screenHeight / this->gridSpacing;

    for (int r = 0; r < num_grid_rows; ++r){
        for (int c = 0; c < num_grid_cols; ++ c){
            this->grid_map[std::make_tuple(r, c)] = 0;
        }
    }

    // mark positions of initial snake head and torso as occupied in the grid_map (unoccupied = 0,
    // occupied = 1);
    // head
    this->grid_map[getGridId(snake.snakeBody.back().rect.x, snake.snakeBody.back().rect.y, this->gridSpacing)] = 1;
    // torso
    this->grid_map[getGridId(snake.snakeBody.front().rect.x, snake.snakeBody.front().rect.y, this->gridSpacing)] = 1;

}

void Game::moveSnake()
{
    // update head position
    BodySegment old_head = snake.snakeBody.back();
    BodySegment new_head = BodySegment(true);
    new_head.rect.x = old_head.rect.x + (snake.direction.x * gridSpacing);
    new_head.rect.y = old_head.rect.y + (snake.direction.y * gridSpacing);
    new_head.rect.w = old_head.rect.w;
    new_head.rect.h = old_head.rect.h;

    // update body positions
    // each segment from the tail to the segment before the head takes the position of the segment in front of it.
    for (snake.snakeBodyIterator = snake.snakeBody.begin(); snake.snakeBodyIterator != std::next(snake.snakeBody.end(), -1); ++snake.snakeBodyIterator){
        snake.snakeBodyIterator->rect.x = std::next(snake.snakeBodyIterator,1)->rect.x;
        snake.snakeBodyIterator->rect.y = std::next(snake.snakeBodyIterator,1)->rect.y;
    }

    // remove old head
    snake.snakeBody.pop_back();
    // add new head
    snake.snakeBody.push_back(new_head);

    outOfBoundsCheck();
}

void Game::outOfBoundsCheck()
{
    // bounds check
    if (snake.snakeBody.back().rect.x > 0)
    {
        snake.snakeBody.back().rect.x = snake.snakeBody.back().rect.x % this->screenWidth;
    }
    else if (snake.snakeBody.back().rect.x < 0)
    {
        snake.snakeBody.back().rect.x = this->screenWidth - snake.snakeBody.back().rect.w;
    }
    if (snake.snakeBody.back().rect.y > 0)
    {
        snake.snakeBody.back().rect.y = snake.snakeBody.back().rect.y % this->screenHeight;
    }
    else if (snake.snakeBody.back().rect.y < 0)
    {
        snake.snakeBody.back().rect.y = this->screenHeight - snake.snakeBody.back().rect.h;
    }

    // update grid map
    // head
    this->grid_map[getGridId(snake.snakeBody.back().rect.x, snake.snakeBody.back().rect.y, this->gridSpacing)] = 1;
    // tail
    this->grid_map[getGridId(snake.snakeBody.front().rect.x, snake.snakeBody.front().rect.y, this->gridSpacing)] = 0;
}

bool Game::collisionCheck()
{
    for (snake.snakeBodyIterator = snake.snakeBody.begin();
         snake.snakeBodyIterator != std::next(snake.snakeBody.end(), -2);
         ++snake.snakeBodyIterator){
            if (SDL_HasIntersection(&(snake.snakeBody.back().rect), &(snake.snakeBodyIterator->rect))){
                printf("Game Over!\nScore: %d\n", snake.snakeBody.size()-1);
                return true;
            }
         }
    return false;
}

void Game::checkFoodConsumption()
{
    if (SDL_HasIntersection(&(snake.snakeBody.back().rect), &(food.rect))){
        int newX = (snake.direction.x != 0) ? snake.snakeBody.front().rect.x + (this->gridSpacing * -1 * snake.direction.x) : snake.snakeBody.front().rect.x;
        int newY = (snake.direction.y != 0) ? snake.snakeBody.front().rect.y + (this->gridSpacing * -1 * snake.direction.y) : snake.snakeBody.front().rect.y;
        snake.snakeBody.push_front((BodySegment){newX, newY, snake.snakeBody.front().rect.w, snake.snakeBody.front().rect.h});
        // update grid cell containing new tail as occupied
        this->grid_map[getGridId(newX, newY, this->gridSpacing)] = 1;
        respawnFood();
    }
}

void Game::respawnFood()
{
    int cellX = -1;
    int cellY = -1;
    bool cell_found = false;

    while (!cell_found)
    {
        // keep guessing random cells on the grid until an unoccupied cell is found (one not containing a snake body elememnt) (i.e grid_map = 0)
        int hRange = this->screenWidth / this->gridSpacing;
        int vRange = this->screenHeight / this->gridSpacing;
        int hGuess = rand() % hRange;
        int vGuess = rand() % vRange;

        if (this->grid_map.find(std::make_tuple(hGuess, vGuess)) != this->grid_map.end()){
            if (this->grid_map.at(std::make_tuple(hGuess, vGuess)) == 0){
                cellX = hGuess * this->gridSpacing;
                cellY = vGuess * this->gridSpacing;
                cell_found = true; // break out of loop
            }
        }
    }
    this->food.rect.x = static_cast<int>(cellX + (this->gridSpacing  * pow(this->food.scale, 2)));
    this->food.rect.y = static_cast<int>(cellY + (this->gridSpacing  * pow(this->food.scale, 2)));
}

void Game::renderGame(SDL_Renderer* renderer)
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF); // black
    SDL_RenderClear(renderer);

    // draw grid lines
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); // white
    // vertical grid lines
    for (int i = this->gridSpacing; i < this->screenWidth; i+=this->gridSpacing)
    {
        SDL_RenderDrawLine(renderer, i, 0, i, this->screenHeight);
    }

    // horizontal grid lines
    for (int j = this->gridSpacing; j < this->screenHeight; j+=this->gridSpacing)
    {
        SDL_RenderDrawLine(renderer, 0, j, this->screenWidth, j);
    }

    // render food
    food.render(renderer);
    // render snake
    snake.render(renderer);
}

void Game::processInput()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W])
            {
                this->snake.direction = {0, -1};
            }

            if (currentKeyStates[SDL_SCANCODE_S])
            {
                this->snake.direction = {0, 1};
            }

            if (currentKeyStates[SDL_SCANCODE_A])
            {
                this->snake.direction = {-1, 0};
            }

            if (currentKeyStates[SDL_SCANCODE_D])
            {
                this->snake.direction = {1, 0};
            }
}

bool Game::run(SDL_Renderer* renderer, const int frameCount, const int speed)
{
    bool quitGame = false;
    processInput();
    if (frameCount % speed == 0)
    {
        moveSnake();
        // if collision with body detected collisionCheck will return true and game will exit
        quitGame = collisionCheck();
        std::cout << "Collision Check: " << collisionCheck() << std::endl;
        checkFoodConsumption();
    }
    renderGame(renderer);
    return quitGame;
}