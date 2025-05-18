#pragma once
#ifndef LULU_ASSETS_HPP
#define LULU_ASSETS_HPP

namespace Lulu::Assets
{

  struct Vec4
  {
    union
    {
      float values[4];
      struct
      {
        float x;
        float y;
        float z;
        float w;
      };

      struct
      {
        float r;
        float g;
        float b;
        float a;
      };
    };

    float &operator[](int idx)
    {
      return values[idx];
    }

    bool operator==(Vec4 other)
    {
      return x == other.x && y == other.y && z == other.z && w == other.w;
    }
  };

  struct Mat4
  {
    union
    {
      Vec4 values[4];
      struct
      {
        float ax;
        float bx;
        float cx;
        float dx;

        float ay;
        float by;
        float cy;
        float dy;

        float az;
        float bz;
        float cz;
        float dz;

        float aw;
        float bw;
        float cw;
        float dw;
      };
    };

    Vec4 &operator[](int col)
    {
      return values[col];
    }
  };

  struct Vec2
  {
    float x;
    float y;

    Vec2 operator/(float scalar)
    {
      return {x / scalar, y / scalar};
    }

    Vec2 operator-(Vec2 other)
    {
      return {x - other.x, y - other.y};
    }

    Vec2 operator+(Vec2 other)
    {
      return {x + other.x, y + other.y};
    }
  };

  struct IVec2
  {
    int x;
    int y;

    Vec2 toVec2()
    {
      return {.x = (float)x, .y = (float)y};
    }

    IVec2 operator-(IVec2 other)
    {
      return {x - other.x, y - other.y};
    }

    IVec2 operator+(IVec2 other)
    {
      return {x + other.x, y + other.y};
    }

    bool operator<(IVec2 other)
    {
      return (x < other.x && y < other.y);
    }

    bool operator<=(IVec2 other)
    {
      return (x <= other.x && y <= other.y);
    }

    bool operator>=(IVec2 other)
    {
      return (x >= other.x && y >= other.y);
    }

    bool operator>(IVec2 other)
    {
      return (x > other.x && y > other.y);
    }
  };

  struct Transform
  {
    IVec2 offset; // Sprite offset in Sprite Sheet (e.g. 0, 0)
    IVec2 size;   // Sprite size in Sprite Sheet (e.g. 16 x 16)

    Vec2 position; // Position in screen
    Vec2 area;     // Size in screen
  };

  struct Sprite
  {
    IVec2 offset;
    IVec2 size;
  };

  Sprite createSprite(IVec2 offset, IVec2 size);
}

#endif // LULU_ASSETS_HPP
