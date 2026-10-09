#include <iostream>

#include "FlowFieldMap.h"
#include "PlayerController.h"
#include "Map.h"
#include "Game.h"
#include "Unit.h"
#include <memory>
#include <optional>

#include "PointOfInterrest.h"

std::shared_ptr<PointOfInterrest> PlayerController::FindCheapestVillageUsingDijkstra(
    std::shared_ptr<Unit> unit, bool allowDiags = false)
{
    std::shared_ptr<Map> map = game->GetMap();
    std::shared_ptr<Tile> startTile = map->GetTile(unit->GetPosition().x, unit->GetPosition().y);
    std::shared_ptr<Tile> currentTile = startTile;

    std::list<std::shared_ptr<Tile>> lockedNodes;
    std::map<std::shared_ptr<Tile>, std::pair<std::shared_ptr<Tile>, int>> infoCost;
    infoCost.emplace(startTile, std::pair<std::shared_ptr<Tile>, int>(startTile, 0));

    std::list<std::pair<CVect, std::shared_ptr<PointOfInterrest>>> villages;
    for (std::shared_ptr<PointOfInterrest> poi : game->GetPointsOfInterest())
    {
        if (poi && poi->GetEntityType() == EntityType::Village)
            villages.emplace_back(poi->GetPosition(), poi);
    }

    const int BLOCKING_TILE_COST = 0x7FFFFFFF;
    
    while (currentTile != nullptr)
    {
        for (std::pair<CVect, std::shared_ptr<PointOfInterrest>> village : villages)
        {
            if (currentTile->GetPosition() == village.first)
                return village.second;
        }
        
#pragma region Direction
        std::list<std::pair<std::shared_ptr<Tile>, int>> availableTile;
        std::vector<std::pair<int, int>> directions;
        directions.emplace_back(1, 0);
        directions.emplace_back(0, 1);
        directions.emplace_back(-1, 0);
        directions.emplace_back(0, -1);
        if (allowDiags)
        {
            directions.emplace_back(1, 1);
            directions.emplace_back(-1, -1);
            directions.emplace_back(1, -1);
            directions.emplace_back(-1, 1);
        }
#pragma endregion
        
        for (std::pair<int, int> dir : directions)
        {
            int x = currentTile->GetPosition().x + dir.first;
            int y = currentTile->GetPosition().y + dir.second;
            if (x < 0 || y < 0 || x >= map->GetWidth() || y >= map->GetHeight())
                continue;

            std::shared_ptr<Tile> neighbour = map->GetTile(x, y);
            if (neighbour == nullptr)
                continue;
            if (neighbour->GetCost() == BLOCKING_TILE_COST)
            {
                std::cout << neighbour->GetCost() << std::endl;   
                continue;
            }

            availableTile.emplace_back(neighbour, neighbour->GetCost());
        }

        for (std::pair<std::shared_ptr<Tile>, int> node : availableTile)
        {
            if (std::find(lockedNodes.begin(), lockedNodes.end(), node.first) != lockedNodes.end())
                continue;

            int myCost = node.second + infoCost[currentTile].second;
            if (infoCost.count(node.first) == 0 || myCost < infoCost[node.first].second)
            {
                infoCost[node.first] = std::pair<std::shared_ptr<Tile>, int>(currentTile, myCost);
            }
        }
        lockedNodes.emplace_back(currentTile);

        // next tile
        int minCost = BLOCKING_TILE_COST;
        currentTile = nullptr;
        for (std::pair<std::shared_ptr<Tile>, std::pair<std::shared_ptr<Tile>, int>> node : infoCost)
        {
            if (std::find(lockedNodes.begin(), lockedNodes.end(), node.first) != lockedNodes.end())
                continue;

            if (node.second.second < minCost)
            {
                currentTile = node.first;
                minCost = node.second.second;
            }
        }
    }

    return nullptr;
}

void PlayerController::ComputePathUsingAStar(std::shared_ptr<Unit> unit, CVect to, bool allowDiags = false)
{
    //This function computes the path from a unit to a destination using A*

    //Temp code, to be replaced by your own code
    std::vector<CVect> path;
    path.push_back(to);
    unit->SetPath(path);
}

void FlowFieldMap::GenerateFlowField(std::shared_ptr<Game> game, CVect cvect, bool useCamps = false,
                                     bool allowDiags = false)
{
    //This function generates the flow field to get to the right destination:
    //if useCamps = false => use cvect
    //if useCamps = true => use game to get the proper camp
    auto map = game->GetMap();
    ResetMap(map->GetWidth(), map->GetHeight());
}

void Adjacent(std::shared_ptr<Tile> tile)
{
}
