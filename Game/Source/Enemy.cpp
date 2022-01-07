#include "List.h"
#include "App.h"


Enemy::Enemy()
{
    this->health = 2;
    this->maxHealth = 2;
}

Enemy::~Enemy()
{
    
}

bool Enemy::Start()
{
    
    return true;
}

bool Enemy::PreUpdate()
{
    return true;
}

bool Enemy::Update(float dt)
{
    
    return true;
}

bool Enemy::PostUpdate()
{
    return false;
}

bool Enemy::CleanUp()
{

    LOG("Freeing pathfinding library");

    lastPath.Clear();
 
    return true;
}

void Enemy::OnCollision(Collider* col)
{
    if (col->type == Type::PLAYER)
    {
        SDL_Rect pCol = col->rect;
        pCol.y = (pCol.y + pCol.h );
        SDL_Rect eCol = this->col->rect;

        if(pCol.y >= eCol.y && pCol.y <= eCol.y + 10)
        {
            if (hurt) return;

            if (health <= 0)
            {
                Death();
            }
            else
            {
                hurt = true;
                health--;
            }
        }
    }
}

void Enemy::WillCollision()
{
    int px = position.x;
    int py = position.y;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = app->map->mapData.layers.start;

    while (mapLayerItem != NULL) {
        if (mapLayerItem->data->properties.GetProperty("Navigation") == 1)
        {
            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);

                    int bx = x * 16;
                    int by = y * 16;
                    int aux = py - JUMPSPEED;
                    switch (gid)
                    {
                    case 243: //Collisions
                    case 247:
                        //UP
                        if (py <= by + 16 && py >= by && px + bounds.w > bx && px < bx + 16)
                        {
                            canMoveDir[UP] = false;
                        }

                        //DOWN
                        if (py + bounds.h >= by && py <= by && px + bounds.w > bx && px < bx + 16)
                        {
                            canMoveDir[DOWN] = false;
                        }

                        //LEFT
                        if (px <= bx + 16 && px >= bx && py + bounds.h > by && py < by + 16)
                        {
                            canMoveDir[LEFT] = false;

                        }

                        //RIGHT
                        if (px + bounds.w >= bx && px <= bx && py + (bounds.h) > by && py < by + 16)
                        {
                            canMoveDir[RIGHT] = false;
                        }
                        while (py + bounds.h > by && py < by && px + bounds.w > bx && px < bx + 16 && !canMoveDir[DOWN] && canMoveDir[UP]) //DOWN
                        {
                            position.y -= 1;
                            py -= 1;
                            break;
                        }
                        while (py <= by + 16 && py >= by && px + bounds.w > bx && px < bx + 16 && canMoveDir[DOWN] && !canMoveDir[UP]) //DOWN
                        {
                            position.y += 1;
                            py += 1;
                            break;
                        }
                        while (px + bounds.w > bx && px < bx && py + bounds.h > by && py < by + 16 && canMoveDir[LEFT] && !canMoveDir[RIGHT] && !canMoveDir[DOWN])
                        {
                            position.x -= 1;
                            px -= 1;
                            break;
                        }
                        while (px < bx + 16 && px > bx && py + bounds.h > by && py < by + 16 && !canMoveDir[LEFT] && canMoveDir[RIGHT] && !canMoveDir[DOWN])
                        {
                            position.x += 1;
                            px += 1;
                            break;
                        }
                        break;
                    case 244: //DETH AREA
                        if (py + bounds.h >= by && py <= by && px + bounds.w > bx && px < bx + 16)
                        {
                            Death();
                        }
                        break;
                    }
                }
            }
        }
        mapLayerItem = mapLayerItem->next;
    }
}

bool Enemy::Death()
{
    dead = true;
    return true;
}

int Enemy::pathFindingA(const iPoint& origin, const iPoint& destination)
{
    if (app->IsWalkable(app->map->WorldToMap(origin.x,origin.y)) && app->IsWalkable(app->map->WorldToMap(destination.x, destination.y)))
    {
        int ret = -1;

        PathList open;
        PathList closed;

        PathNode originNode;

        originNode.pos = app->map->WorldToMap(origin.x, origin.y);

        open.list.add(originNode);
        while (open.list.count() > 0)
        {
            ListItem<PathNode>* lowest = open.GetNodeLowestScore();
            ListItem<PathNode>* node = closed.list.add(lowest->data);
            open.list.del(open.GetNodeLowestScore());
            if (node->data.pos == app->map->WorldToMap(destination.x, destination.y))
            {
                lastPath.Clear();

                // Backtrack to create the final path
                // Use the Pathnode::parent and Flip() the path when you are finish
                const PathNode* pathNode = &node->data;

                while (pathNode)
                {
                    lastPath.PushBack(pathNode->pos);
                    pathNode = pathNode->parent;
                }

                lastPath.Flip();
                ret = lastPath.Count();
                break;
            }

            // L12b: TODO 5: Fill a list of all adjancent nodes
            PathList Adjancent;
            node->data.FindWalkableAdjacents(Adjancent);
            ListItem<PathNode>* item = Adjancent.list.start;
            for (; item; item = item->next)
            {
                // L12b: TODO 6: Iterate adjancent nodes:
                // ignore nodes in the closed list
                if (closed.Find(item->data.pos) != NULL)
                {
                    continue;
                }
                // If it is NOT found, calculate its F and add it to the open list
                ListItem<PathNode>* adjacentInOpen = open.Find(item->data.pos);
                if (adjacentInOpen == NULL)
                {
                    item->data.CalculateF(app->map->WorldToMap(destination.x, destination.y));
                    open.list.add(item->data);
                }
                // If it is already in the open list, check if it is a better path (compare G)
                // If it is a better path, Update the parent
                else
                {
                    if (adjacentInOpen->data.g > item->data.g + 1)
                    {
                        adjacentInOpen->data.parent = item->data.parent;
                        adjacentInOpen->data.CalculateF(app->map->WorldToMap(destination.x, destination.y));
                    }
                }
            }
        }
        return lastPath.Count();
    }
    else
    {
        return -1;
    }
}

const DynArray<iPoint>* Enemy::GetLastPath()
{
    
     return &lastPath;
    
}

PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{
}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{
}

PathNode::PathNode(const PathNode& node): g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{
}

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) 
{
    iPoint cell;
    uint before = list_to_fill.list.count();

    // north
    cell.create(pos.x, pos.y + 1);
    if (app->IsWalkable(cell))
        list_to_fill.list.add(PathNode(-1, -1, cell, this));

    // south
    cell.create(pos.x, pos.y - 1);
    if (app->IsWalkable(cell))
        list_to_fill.list.add(PathNode(-1, -1, cell, this));

    // east
    cell.create(pos.x + 1, pos.y);
    if (app->IsWalkable(cell))
        list_to_fill.list.add(PathNode(-1, -1, cell, this));

    // west
    cell.create(pos.x - 1, pos.y);
    if (app->IsWalkable(cell))
        list_to_fill.list.add(PathNode(-1, -1, cell, this));

    return list_to_fill.list.count();


    return uint();
}

int PathNode::Score () const
{
    return g+h;
}

int PathNode::CalculateF(const iPoint& destination)
{
   
    g = parent->g + 1;
    h = pos.DistanceTo(destination);
     
    return g + h;
}

ListItem<PathNode>* PathList::Find(const iPoint& point) const
{
    ListItem<PathNode>* item = list.start;
    while (item)
    {
        if (item->data.pos == point)
            return item;
        item = item->next;
    }
    return item;
}

ListItem<PathNode>* PathList::GetNodeLowestScore() const
{
    ListItem<PathNode>* ret = NULL;
    int min = 65535;

    ListItem<PathNode>* item = list.end;
    while (item)
    {
        if (item->data.Score() < min)
        {
            min = item->data.Score();
            ret = item;
        }
        item = item->prev;
    }
    return ret;
}

void Enemy::stateMachine()
{

}

void Enemy::Save(pugi::xml_node& data) const
{
    pugi::xml_node i = data.child("enemies").append_child("e");
    i.append_attribute("posX") = position.x;
    i.append_attribute("posY") = position.y;
    i.append_attribute("id") = (int) enemy_id;
}