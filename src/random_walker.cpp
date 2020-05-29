#include "random_walker.h"
#include <set>
#include "misc_utils.h"

random_walker_t::random_walker_t(glm::dvec2 position, context_t *ctx, namer_t required_pathfinding_property, value_t required_pathfinding_min_value, value_t required_pathfinding_max_value, value_t required_pathfinding_def_value) : walker_t(position, ctx)
{
  this->required_pathfinding_property = required_pathfinding_property;
  this->required_pathfinding_min_value = required_pathfinding_min_value;
  this->required_pathfinding_max_value = required_pathfinding_max_value;
  this->required_pathfinding_def_value = required_pathfinding_def_value;
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
    double backwards = direction + M_PI;
    while(backwards > 2 * M_PI)
    {
      backwards -= 2 * M_PI;
    }
    for(double new_direction = dir_east; new_direction < dir_south_east; new_direction += M_PI / 2)
    {
      if(eq(new_direction, backwards))
      {
        continue;
      }
      value_t pathfinding_value = this->ctx->get_grid()->at(glm::ivec2(position.x + cos(new_direction) * 1.10, position.y + sin(new_direction) * 1.10))->get(required_pathfinding_property, required_pathfinding_def_value);
      if(required_pathfinding_min_value <= pathfinding_value && pathfinding_value <= required_pathfinding_max_value)
      {
        possible_directions.insert(new_direction);
      }
    }
    if(possible_directions.size() == 0)
    {
      value_t pathfinding_value = this->ctx->get_grid()->at(glm::ivec2(position.x + cos(backwards) * 1.10, position.y + sin(backwards) * 1.10))->get(required_pathfinding_property, required_pathfinding_def_value);
      if(required_pathfinding_min_value <= pathfinding_value && pathfinding_value <= required_pathfinding_max_value)
      {
        possible_directions.insert(backwards);
      }
    }
    if(possible_directions.size() == 0)
    {
      dist_to_parse = 0;
      wd.suicide = true;
    }
    else
    {
      int pos = rand() % possible_directions.size();
      auto it = possible_directions.begin();
      while(pos--)
      {
        it++;
      }
      direction = *it;
    }
  }
  position = position + glm::dvec2(cos(direction), sin(direction)) * dist_to_parse;
  wd.delta_position = position - this->position;
  wd.delta_direction = direction - this->direction;
  return wd;
}
