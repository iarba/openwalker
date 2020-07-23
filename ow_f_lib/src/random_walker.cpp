#include "random_walker.h"
#include "named_virtual_pointer.h"
#include <set>

def(cloner_registry, random_walker_cloner);

void random_walker_t::load()
{
  imp(cloner_registry, random_walker_cloner);
  cloner_t::g_cloner_get()->reg_walker(cloner_registry__random_walker_cloner, new random_walker_t::random_walker_constructor());
}

walker_t *random_walker_t::random_walker_constructor::instantiate(walker_t *w)
{
  random_walker_t *rw = (random_walker_t *)w;
  random_walker_t *nrw;
  if(rw->use_nvp)
  {
    nrw = new random_walker_t(rw->position, rw->pathfinding_nvp);
  }
  else
  {
    nrw = new random_walker_t(rw->position, rw->required_pathfinding_property, rw->required_pathfinding_min_value, rw->required_pathfinding_max_value, rw->required_pathfinding_def_value);
  }
  rw->copy_into(nrw);
  return nrw;
}

walker_t *random_walker_t::random_walker_constructor::deserialise(std::istream &is)
{
  walker_t *nrw = new random_walker_t(is);
  return nrw;
}

random_walker_t::random_walker_t(glm::dvec2 position, namer_t required_pathfinding_property, value_t required_pathfinding_min_value, value_t required_pathfinding_max_value, value_t required_pathfinding_def_value) : walker_t(position)
{
  this->required_pathfinding_property = required_pathfinding_property;
  this->required_pathfinding_min_value = required_pathfinding_min_value;
  this->required_pathfinding_max_value = required_pathfinding_max_value;
  this->required_pathfinding_def_value = required_pathfinding_def_value;
  clone_identifier = cloner_registry__random_walker_cloner;
  use_nvp = false;
  regenerate_pathfinding_checker();
}

random_walker_t::random_walker_t(glm::dvec2 position, namer_t pathfinding_nvp) : walker_t(position)
{
  clone_identifier = cloner_registry__random_walker_cloner;
  this->pathfinding_nvp = pathfinding_nvp;
  use_nvp = true;
  regenerate_pathfinding_checker();
}

random_walker_t::random_walker_t(std::istream &is) : walker_t(is)
{
  ow_assert(is >> use_nvp)
  if(use_nvp)
  {
    ow_assert(is >> pathfinding_nvp);
  }
  else
  {
    ow_assert(is >> required_pathfinding_property);
    ow_assert(is >> required_pathfinding_min_value);
    ow_assert(is >> required_pathfinding_max_value);
    ow_assert(is >> required_pathfinding_def_value);
  }
  regenerate_pathfinding_checker();
  clone_identifier = cloner_registry__random_walker_cloner;
}

random_walker_t::~random_walker_t()
{
}

void random_walker_t::serialise(std::ostream &os) const
{
  this->walker_t::serialise(os);
  os << " " << use_nvp << " ";
  if(use_nvp)
  {
    os << " " << pathfinding_nvp;
  }
  else
  {
    os << " " << required_pathfinding_property << " ";
    os << " " << required_pathfinding_min_value << " ";
    os << " " << required_pathfinding_max_value << " ";
    os << " " << required_pathfinding_def_value << " ";
  }
}

void random_walker_t::copy_into(random_walker_t *other) const
{
  this->walker_t::copy_into(other);
  other->use_nvp = this->use_nvp;
  if(use_nvp)
  {
    other->pathfinding_nvp = this->pathfinding_nvp;
  }
  else
  {
    other->required_pathfinding_property = this->required_pathfinding_property;
    other->required_pathfinding_min_value = this->required_pathfinding_min_value;
    other->required_pathfinding_max_value = this->required_pathfinding_max_value;
    other->required_pathfinding_def_value = this->required_pathfinding_def_value;
  }
  other->regenerate_pathfinding_checker();
}

glm::ivec2 forward(glm::dvec2 position, double direction)
{
  glm::ivec2 rval;
  double eps = 0.0001;
  if(cos(direction) > eps)
  {
    rval.x = std::floor(position.x + 1 + eps);
  }
  else
  {
    if(cos(direction) < -eps)
    {
      rval.x = std::floor(position.x - eps);
    }
    else
    {
      rval.x = position.x;
    }
  }
  if(sin(direction) > eps)
  {
    rval.y = std::floor(position.y + 1 + eps);
  }
  else
  {
    if(sin(direction) < -eps)
    {
      rval.y = std::floor(position.y - eps);
    }
    else
    {
      rval.y = position.y;
    }
  }
  return rval;
}

walker_delta *random_walker_t::compute_delta(context_t ctx) const
{
  double eps = 0.0001;
  walker_delta *wd = this->walker_t::compute_delta(ctx);
  double dist_to_parse = this->speed;
  double direction = this->direction;
  glm::dvec2 position = this->position;
  double distance;
  for(glm::dvec2 next_grid_point = forward(position, direction); (distance = glm::distance(position, next_grid_point)) <= dist_to_parse + eps; next_grid_point = forward(position, direction))
  {
    if(dist_to_parse <= eps)
    {
      break;
    }
    dist_to_parse -= distance;
    position = next_grid_point;
    std::set<direction_t> possible_directions;
    double backwards = direction + M_PI;
    while(backwards >= 2 * M_PI)
    {
      backwards -= 2 * M_PI;
    }
    for(double new_direction = dir_east; new_direction < dir_south_east; new_direction += M_PI / 2)
    {
      if(eq(new_direction, backwards))
      {
        continue;
      }
      ctx.cell_pos = forward(position, new_direction);
      if(pathfinding_checker(ctx))
      {
        possible_directions.insert(new_direction);
      }
    }
    if(possible_directions.size() == 0)
    {
      ctx.cell_pos = forward(position, backwards);
      if(pathfinding_checker(ctx))
      {
        possible_directions.insert(backwards);
      }
    }
    if(possible_directions.size() == 0)
    {
      dist_to_parse = 0;
      wd->suicide = true;
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
  wd->delta_position = position - this->position;
  wd->delta_direction = direction - this->direction;
  return wd;
}

std::function<bool(context_t)> random_walker_t::get_pathfinding_checker()
{
  return pathfinding_checker;
}

void random_walker_t::regenerate_pathfinding_checker()
{
  if(use_nvp)
  {
    pathfinding_checker = *(std::function<bool(context_t)> *)dynamic_named_pointer_manager_t::g_dnpm_get()->get(pathfinding_nvp);
  }
  else
  {
    auto rpp = required_pathfinding_property;
    auto rpmv = required_pathfinding_min_value;
    auto rpMv = required_pathfinding_max_value;
    auto rpdv = required_pathfinding_def_value;
    pathfinding_checker = [rpp, rpmv, rpMv, rpdv](context_t ctx){value_t pathfinding_value = ctx.world->get_grid(ctx.grid_id)->at(ctx.cell_pos)->get(rpp, rpdv, NULL); return pathfinding_value > rpmv && pathfinding_value < rpMv;};
  }
}

