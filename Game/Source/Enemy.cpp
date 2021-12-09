#include "List.h"
#include "App.h"


Enemy::Enemy()
{

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
    return true;
}

void Enemy::OnCollision(Collider* col)
{
}

void Enemy::WillCollision()
{
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

//bool Enemy::IsWalkable(iPoint pos)
//{
//    ListItem<MapLayer*>* mapLayerItem;
//    mapLayerItem = app->map->mapData.layers.start;
//
//    while (mapLayerItem != NULL) {
//        if (mapLayerItem->data->properties.GetProperty("Navigation") == 1)
//        {
//            if (mapLayerItem->data->Get(pos.x, pos.y) == 243)
//            {
//                return false;
//            }
//        }
//        mapLayerItem = mapLayerItem->next;
//    }
//
//    return true;
//}

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
