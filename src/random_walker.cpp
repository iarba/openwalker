#include "random_walker.h"
#include <set>

random_walker_t::random_walker_t(glm::dvec2 position, context_t *ctx, namer_t required_pathfinding_property, value_t required_pathfinding_value) : walker_t(position, ctx)
{
  this->required_pathfinding_property = required_pathfinding_property;
  this->required_pathfinding_value = required_pathfinding_value;
}

walker_delta random_walker_t::compute_delta() const
{
  walker_delta wd = this->walker_t::compute_delta();
  double dist_to_parse = this->speed;
  double direction = this->direction;
  glm::dvec2 position = this->position;
  double distance;
  glm::ivec2 grid_position;
  for(glm::dvec2 next_grid_point = position; (distance = glm::distance(position, next_grid_point)) < dist_to_parse; next_grid_point = glm::dvec2(grid_position = glm::ivec2(position + glm::dvec2(cos(direction), sin(direction)))))
  {
    dist_to_parse -= distance;
    position = next_grid_point;
    std::set<direction_t> possible_directions;
    // for each direction, if it's not backwards, consider going in that direction
    if(possible_directions.size() == 0)
    {
      // consider going backwards
    }
    if(possible_directions.size() == 0)
    {
      dist_to_parse = 0;
      wd.suicide = true;
    }
    else
    {
      // choose a direction at random from the set;
    }
  }
  position = position + glm::dvec2(cos(direction), sin(direction)) * dist_to_parse;
  wd.delta_position = position - this->position;
  wd.delta_direction = direction - this->direction;
  return wd;
}
