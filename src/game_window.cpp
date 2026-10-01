/*
*   game_window.cpp
*
*   this is a really useful comment ! /jk
*   not cleanest code but i just wanna move to the next task for now LOLLLL
*   author : yolicap
*/

#include "game.h"
#include "game_window.h"
#include <numbers>

GameWindow::GameWindow(olc::vf2d p, float w, float h, olc::Image* wtc, olc::Image* wbc, olc::Image* wte, olc::Image* we, olc::Image* wbu, olc::Image* wc) {
    position = p;
    width = w;
    height = h;
    windowTopCorner = wtc;
    windowBottomCorner = wbc;
    windowTopEdge = wte;
    windowEdge = we;
    windowButtonUp = wbu;
    windowContent = wc;
}

void GameWindow::Draw(olc::Draw& draw) {
    const auto origin = olc::vf2d{0.0f, 0.0f};

    float x1, y1, x2, y2;
    x1 = position.x;
    y1 = position.y;
    x2 = position.x + width;
    y2 = position.y + height;

    // Draw background
    draw.Image(windowContent->region({0,0}, {width, height-windowTopCorner->Size().y}), {x1+windowTopCorner->Size().x, y1+windowTopCorner->Size().y});

    DrawEdges(draw, x1, y1, x2, y2);
    DrawCorners(draw, x1, y1, x2, y2);

    // Draw button
    draw.Image(*windowButtonUp, {x2-windowButtonUp->Size().x+windowTopCorner->Size().x, y1});
    
    // Set background
    draw.SetTarget(*windowContent);
    draw.Clear(olc::Pixel(0xe3, 0xf5, 0xf1));
    draw.SetTarget(*(GameParameters.screen));
}

void GameWindow::DrawEdges(olc::Draw& draw, float x1, float y1, float x2, float y2) {
    auto batch = draw.CreateImageBatch(*windowEdge);

    // bottom edge
    draw.Image(
        batch,
        *windowEdge, 
        {x1+windowBottomCorner->Size().x, y2}, 
        {(x2-x1-windowBottomCorner->Size().x) / windowEdge->Size().x, 1.0f}
    );

    // right edge
    draw.ImageRotated(
        // batch, // TODO: bug (?) wont let me use batch. scaling ends up being done incorrectly
        *windowEdge, 
        {x2, y2}, 
        (std::numbers::pi_v<float> / 2.0f) * -1.0f, 
        { 0.0f, 0.0f},  
        { (y2-y1-windowTopCorner->Size().y)/windowEdge->Size().x, 1.0f }
    );

    // left edge 
    draw.ImageRotated(
        // batch, 
        *windowEdge, 
        {x1, y1+windowTopCorner->Size().y}, 
        (std::numbers::pi_v<float> / 2.0f), 
        { 0.0f, static_cast<float>(windowEdge->Size().y) }, 
        { (y2-y1-windowTopCorner->Size().y) / windowEdge->Size().x, 1.0f }
    );
    draw.Batch(batch);

    // top edge
    draw.Image(
        *windowTopEdge, 
        {x1+windowTopCorner->Size().x, y1}, 
        {(x2-x1-windowTopCorner->Size().x) / windowTopEdge->Size().x, 1.0f}
    );
}

void GameWindow::DrawCorners(olc::Draw& draw, float x1, float y1, float x2, float y2) {
    // Draw corners
    auto batch = draw.CreateImageBatch(*windowTopCorner);
    draw.Image(batch, *windowTopCorner, {x1, y1});
    draw.Image(batch, (*windowTopCorner).flipH(), {x2, y1});
    draw.Batch(batch);

    batch = draw.CreateImageBatch(*windowBottomCorner);
    draw.Image(batch, *windowBottomCorner, {x1, y2});
    draw.Image(batch, (*windowBottomCorner).flipH(), {x2, y2});
    draw.Batch(batch);
}