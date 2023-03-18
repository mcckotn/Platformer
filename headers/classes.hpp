#pragma once
#include <iostream>
#include <raylib.h>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <cmath>
#include <sstream>
#include <functional>

namespace platformer
{
    // All objects that are collidable must be derived from this parent class
    // Everything is in charge of drawing itself
    // No collidable should be deleted or added UNTIL ALL THREADS are done accessing it
    class collidable
    {
    protected:
        Rectangle inGamePositionDimension{0, 0, 64, 64};
        bool collisionEnabled;
        int groupNumber;

    public:
        // Yet to be implemented
        void updateGroupNumber(void *ptr)
        {
        }
        // So far this number does nothing
        void setGroupNumber(int gn)
        {
            groupNumber = gn;
        }
        // So far this number does nothing
        int getGroupNumber()
        {
            return groupNumber;
        }
        // This position is in game world space
        void setPosition(float x, float y)
        {
            inGamePositionDimension.x = x;
            inGamePositionDimension.y = y;
        }
        // These dimentions are for world space
        void setDimentions(float width, float height)
        {
            inGamePositionDimension.width = width;
            inGamePositionDimension.height = height;
        }
        // This position is in game world space
        Vector2 getPosition()
        {
            return {inGamePositionDimension.x, inGamePositionDimension.y};
        }
        // These dimentions are for world space
        Vector2 getDimentions()
        {
            return {inGamePositionDimension.width, inGamePositionDimension.height};
        }
        Rectangle getRectangle()
        {
            return inGamePositionDimension;
        }
        collidable()
        {
            inGamePositionDimension = {0.0f, 0.0f, 0.0f, 0.0f};
            collisionEnabled = 1;
            groupNumber = rand();
        }
    };
    class stationaryStaticBlock : public collidable
    {
    protected:
        Rectangle positionOnSpriteSheet;
        int type;

    public:
        void setPositionOnSpriteSheet(Rectangle rect)
        {
            positionOnSpriteSheet = rect;
        }
        Rectangle getPositionOnSpriteSheet()
        {
            return positionOnSpriteSheet;
        }
        void draw(Texture2D &spritesheet)
        {
            DrawTexturePro(spritesheet, positionOnSpriteSheet, inGamePositionDimension, {0, 0}, 0, WHITE);
        }
        stationaryStaticBlock()
        {
        }
        stationaryStaticBlock(stationaryStaticBlock &whereToInherit, int globalx, int globaly, int wid, int hgt)
        {
            (*this) = whereToInherit;
            inGamePositionDimension.x = globalx;
            inGamePositionDimension.y = globaly;
            inGamePositionDimension.width = wid;
            inGamePositionDimension.height = hgt;
        }
        void setType(int t)
        {
            type = t;
        }
        int getType()
        {
            return type;
        }
    };
    class stationaryAnimatedBlock : public collidable
    {
    protected:
        Rectangle initialPositionOnSpriteSheet;
        Vector2 pixelsToOffsetUponUpdate;
        int maximumFrames = 2;
        int frameToDisplay{0};
        size_t *iterable = nullptr;
        int type;

    public:
        void setInitialPositionOnSpriteSheet(Rectangle rect)
        {
            initialPositionOnSpriteSheet = rect;
        }
        void setPixelsToOffset(int dx, int dy)
        {
            pixelsToOffsetUponUpdate.x = dx;
            pixelsToOffsetUponUpdate.y = dy;
        }
        void setMaxFrames(int f)
        {
            maximumFrames = f;
        }
        void setIterablePointer(size_t *ptr)
        {
            iterable = ptr;
        }
        Rectangle getInitialSpritePosition()
        {
            return initialPositionOnSpriteSheet;
        }
        int getFrameDisplayed()
        {
            return frameToDisplay;
        }
        void draw(Texture2D &spritesheet)
        {
            if (iterable != nullptr)
            {
                frameToDisplay = (*iterable % maximumFrames);
                DrawTexturePro(spritesheet, {(frameToDisplay * pixelsToOffsetUponUpdate.x) + initialPositionOnSpriteSheet.x, (frameToDisplay * pixelsToOffsetUponUpdate.y) + initialPositionOnSpriteSheet.y, initialPositionOnSpriteSheet.width, initialPositionOnSpriteSheet.height}, inGamePositionDimension, {0, 0}, 0, WHITE);
            }
            else
            {
                std::cerr << "Object is missing iterable" << '\n';
            }
        }
        stationaryAnimatedBlock()
        {
        }
        stationaryAnimatedBlock(stationaryAnimatedBlock &whereToInherit, int globalx, int globaly, int wid, int hgt, size_t *i)
        {
            (*this) = whereToInherit;
            inGamePositionDimension.x = globalx;
            inGamePositionDimension.y = globaly;
            inGamePositionDimension.width = wid;
            inGamePositionDimension.height = hgt;
            iterable = i;
        }
        void setType(int t)
        {
            type = t;
        }
        int getType()
        {
            return type;
        }
    };
    class npc : public stationaryAnimatedBlock
    {
    protected:
    };
    class player : public stationaryAnimatedBlock
    {
    protected:
        int isFacingLeft{0}; // Set to 64 to face right
        int speed{64};
        Vector2 velocity;
        Vector2 terminalVelocity{256.0f, 512.0f};
        Vector2 dragCoefficent{100.0f, 128.0f};
        bool canJump{1};
        int isMoving{0};
        Vector2 playerDesiredMovement;

    public:
        void setDesiredMovement(float x, float y)
        {
            playerDesiredMovement.x = x;
            playerDesiredMovement.y = y;
        }
        void incrementDesiredMovement(float x, float y)
        {
            playerDesiredMovement.x += x;
            playerDesiredMovement.y += y;
        }
        void decrementDesiredMovement(float x, float y)
        {
            playerDesiredMovement.x -= x;
            playerDesiredMovement.y -= y;
        }
        player()
        {
            velocity = {0.0f, 0.0f};
            playerDesiredMovement = {0.0f, 0.0f};
            inGamePositionDimension.width = 64;
            inGamePositionDimension.height = 64;
        }
        player(int spd, int tmvX, int tmvY, int drgcX, int drgcY)
        {
            speed = spd;
            velocity = {0.0f, 0.0f};
            playerDesiredMovement = {0.0f, 0.0f};
            terminalVelocity.x = tmvX;
            terminalVelocity.y = tmvY;
            dragCoefficent.x = drgcX;
            dragCoefficent.y = drgcY;
        }
        // Set val to be 64 to face right
        void setFaceDirection(char val)
        {
            isFacingLeft = val;
        }
        int getSpeed()
        {
            return speed;
        }
        Rectangle getPredictedPosition(float timeDelta, int xAxisOverride, int yAxisOverride)
        {
            // This is slightly smaller than the actual sprite because floating point approximation limitations
            return {(inGamePositionDimension.x) + (velocity.x * timeDelta * xAxisOverride), (inGamePositionDimension.y + 23) + (velocity.y * timeDelta * yAxisOverride), 63, 41};
        }
        void doPhysicsStep(std::vector<stationaryStaticBlock *> &staticBlocks, std::vector<stationaryAnimatedBlock *> &animatedBlocks, float frameDelta)
        {
            velocity.y += speed * dragCoefficent.y * frameDelta;
            velocity.x > 0 ? velocity.x -= 1 *dragCoefficent.x *frameDelta : velocity.x += 1 * dragCoefficent.x * frameDelta;
            velocity.x += playerDesiredMovement.x;
            velocity.y += playerDesiredMovement.y;
            if (velocity.x > terminalVelocity.x)
            {
                velocity.x = terminalVelocity.x;
            }
            if (velocity.y > terminalVelocity.y)
            {
                velocity.y = terminalVelocity.y;
            }
            if (velocity.x < terminalVelocity.x * -1)
            {
                velocity.x = terminalVelocity.x * -1;
            }
            if (velocity.y < terminalVelocity.y * -1)
            {
                velocity.y = terminalVelocity.y * -1;
            }
            playerDesiredMovement = {0, 0};
            bool xAxisWillCollide;
            bool yAxisWillCollide;
            bool cWillCollide;
            int indexes[3] = {-1, -1, -1};
            for (size_t i = 0; i < staticBlocks.size() && indexes[0] == -1; i++)
            {
                xAxisWillCollide = CheckCollisionRecs(getPredictedPosition(frameDelta, 1, 0), staticBlocks.at(i)->getRectangle());
                if (xAxisWillCollide)
                {
                    indexes[0] = i;
                    break;
                }
            }
            for (size_t i = 0; i < staticBlocks.size() && indexes[1] == -1; i++)
            {

                yAxisWillCollide = CheckCollisionRecs(getPredictedPosition(frameDelta, 0, 1), staticBlocks.at(i)->getRectangle());
                if (yAxisWillCollide)
                {
                    canJump = 1;
                    break;
                }
            }
            for (size_t i = 0; i < staticBlocks.size() && indexes[2] == -1; i++)
            {
                cWillCollide = CheckCollisionRecs(getPredictedPosition(frameDelta, 1, 1), staticBlocks.at(i)->getRectangle());
                if (cWillCollide)
                {
                    indexes[2] = i;
                    break;
                }
            }
            std::abs(velocity.x) > 0.3f ? velocity.x = velocity.x : velocity.x = 0;
            inGamePositionDimension.x += (velocity.x * frameDelta * !xAxisWillCollide);
            inGamePositionDimension.y += (velocity.y * frameDelta * !yAxisWillCollide);
            velocity.x = velocity.x * !xAxisWillCollide;
            velocity.y = velocity.y * !yAxisWillCollide;
        }
    };
    class console
    {
    protected:
        Vector2 positionToStartDrawing;
        std::string cin;
        std::vector<std::string> arguments;
        bool isInConsole;
        void parseArguments()
        {
            arguments.clear();
            std::string buffer;
            std::stringstream otherCin(cin);
            while (std::getline(otherCin, buffer, ' '))
            {
                arguments.push_back(buffer);
            }
        }

    public:
        void draw(Vector2 &windowResolution, float &hypotenuse, wchar_t &keypress)
        {
            {
                if (keypress != 0)
                {
                    cin += (keypress % 255);
                }
                if (IsKeyPressed(KEY_BACKSPACE) && cin.size() > 0)
                {
                    cin.pop_back();
                }
                if (IsKeyPressed(KEY_ENTER))
                {
                }
            }
            if (isInConsole)
            {
                DrawText(cin.c_str(), (positionToStartDrawing.x * windowResolution.x), (positionToStartDrawing.y * windowResolution.y), hypotenuse * 0.01f, BLUE);
            }
        }
        console()
        {
            positionToStartDrawing = {0.1f, 0.8f};
            isInConsole = 0;
        }
        void toggleConsole()
        {
            isInConsole = !isInConsole;
        }
    };
    class setting
    {
    protected:
        std::string humanReadableName;
        int value{-1};

    public:
        setting(const char *name, int val)
        {
            value = val;
            humanReadableName = name;
        }
        setting()
        {
        }
        void setValue(int val)
        {
            value = val;
        }
        int getValue()
        {
            return value;
        }
        std::string getName()
        {
            return humanReadableName;
        }
        void setName(const char *name)
        {
            humanReadableName = name;
        }
        void setAll(const char *name, int val)
        {
            value = val;
            humanReadableName = name;
        }
    };
}