#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "VBOWrapper.h"

struct Character {
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
	float textureCoord;
};

struct TextObject
{
	int id;
	float x, y;
	float scale;
	std::string content;

	std::vector<TextVertexAttrib> vertices = std::vector<TextVertexAttrib>();

	TextObject(int id, float x, float y, float scale, std::string content, std::vector<TextVertexAttrib> &vertices)
	{
		this->id = id;
		this->x = x;
		this->y = y;
		this->scale = scale;
		this->content = content;
		this->vertices = vertices;
	}
};


class TextHandler
{
public:
    unsigned int textAtlasWidth, textAtlasHeight;
    Character characters[128];

    std::vector<TextVertexAttrib> createTextVertices(std::string text, float x, float y, float scale)
    {
        std::vector<TextVertexAttrib> attribs;
        attribs.reserve(6 * text.size());

        // iterate through all characters
        std::string::const_iterator c;
        int i = 0;

        float halfPixelAdjustment = 1.0 / (textAtlasWidth);

        float initialX = x;


        for (c = text.begin(); c != text.end(); c++)
        {
            if (*c == '\n') //Move position down for a newline character
            {
                y -= textAtlasHeight * scale;
                x = initialX;
                continue;
            }

            Character ch = characters[*c];
            float x2 =  x + ch.bearing[0] * scale;
            float y2 = -y - ch.bearing[1] * scale;
            float w = ch.size[0] * scale;
            float h = ch.size[1] * scale;

            // Advance the cursor to the start of the next character 
            x += ch.advance * scale;
            y += 0 * scale;

            // Skip glyphs that have no pixels
            if(!w || !h)
                continue;

            attribs.push_back(TextVertexAttrib(x2,     -y2    , ch.textureCoord,                                            0.0f));
            attribs.push_back(TextVertexAttrib(x2 + w, -y2    , ch.textureCoord + float(ch.size[0]) / textAtlasWidth - halfPixelAdjustment,   0.0f));
            attribs.push_back(TextVertexAttrib(x2,     -y2 - h, ch.textureCoord,                                          float(ch.size[1]) / textAtlasHeight));
            attribs.push_back(TextVertexAttrib(x2 + w, -y2    , ch.textureCoord + float(ch.size[0]) / textAtlasWidth - halfPixelAdjustment,   0.0f));
            attribs.push_back(TextVertexAttrib(x2,     -y2 - h, ch.textureCoord,                                          float(ch.size[1]) / textAtlasHeight));
            attribs.push_back(TextVertexAttrib(x2 + w, -y2 - h, ch.textureCoord + float(ch.size[0]) / textAtlasWidth - halfPixelAdjustment,  float(ch.size[1]) / textAtlasHeight));

        }
        return attribs;
    }
    TextHandler(/* args */)
    {

    }
    ~TextHandler()
    {

    }
private:

};