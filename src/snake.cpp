#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
  // Calculate the new head position based on the current direction
  float new_head_x = head_x;
  float new_head_y = head_y;

  switch (direction) {
    case Direction::kUp:
      new_head_y -= speed;
      break;

    case Direction::kDown:
      new_head_y += speed;
      break;

    case Direction::kLeft:
      new_head_x -= speed;
      break;

    case Direction::kRight:
      new_head_x += speed;
      break;
  }

  // Check if the new direction is opposite to the current direction.
  // If so, ignore the input to prevent premature termination.
  if (!((direction == Direction::kUp && new_head_y > head_y) ||
        (direction == Direction::kDown && new_head_y < head_y) ||
        (direction == Direction::kLeft && new_head_x > head_x) ||
        (direction == Direction::kRight && new_head_x < head_x))) {
    // Update the head position if the new direction is valid.
    head_x = new_head_x;
    head_y = new_head_y;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}


void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}