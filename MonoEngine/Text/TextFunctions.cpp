//
//  TextFunctions.cpp
//  Mono1
//
//  Created by Niblit on 2012-07-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TextFunctions.h"
#include "SysOpenGL.h"
#include "SysFile.h"
#include "ITexture.h"
#include "TextureFactory.h"
#include "Vector2f.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <map>
#include <iostream>
#include <cmath>


namespace
{
    struct CharData
    {
        float xadvance;
        float xoffset;
        float yoffset;
        float width;
        float height;
        
        float texCoordX0;
        float texCoordY0;
        float texCoordX1;
        float texCoordY1;
    };

    static mono::ITexturePtr fontTexture;
    
    typedef std::map<char, CharData> CharDataMap;
    static CharDataMap charMap;

    math::Vector2f MeasureString(const std::string& text)
    {
        math::Vector2f size;
        
        for(std::string::const_iterator it = text.begin(), end = text.end(); it != end; ++it)
        {
            const char currentChar = *it;
            
            // Look up char in map.
            const CharDataMap::const_iterator foundChar = charMap.find(currentChar);
            if(foundChar == charMap.end())
                continue;
            
            const CharData& data = foundChar->second;
            size.mX += data.xadvance;
            size.mY = std::max(data.height, size.mY);
        }
        
        return size;
    }
    
}

void mono::LoadFont(const std::string& font, float size, float scale)
{
    File::FilePtr fontfile = File::OpenBinaryFile(font);
    const long filesize = File::FileSize(fontfile);
    
    ByteVector fontbuffer(filesize, 0);
    fread(&fontbuffer.front(), 1, filesize, fontfile.get());
    
    const int width = 512;
    const int height = 512;
    const int base = 32;
    const int chars = 224;

    byte bitmap[width * height];
    stbtt_bakedchar chardata[chars];
    
    // The bitmap generated by this function has origo in the upper left corner,
    // thats why a bit down the y texture coordinate is flipped.
    stbtt_BakeFontBitmap(&fontbuffer.front(), 0, size, bitmap, width, height, base, chars, chardata);    
    fontTexture = mono::CreateTexture(bitmap, width, height, GL_ALPHA, GL_ALPHA);
    

    const float texCoordXMulti = 1.0f / width;
    const float texCoordYMulti = 1.0f / height;
    
    for(int index = 0; index < chars; ++index)
    {
        const stbtt_bakedchar& bakedchar = chardata[index];
        
        CharData data;
        data.width = (bakedchar.x1 - bakedchar.x0) * scale;
        data.height = (bakedchar.y1 - bakedchar.y0) * scale;
        data.xadvance = bakedchar.xadvance * scale;
        data.xoffset = bakedchar.xoff * scale;
        data.yoffset = (bakedchar.yoff * scale) + data.height;
        data.texCoordX0 = bakedchar.x0 * texCoordXMulti;
        data.texCoordY0 = bakedchar.y1 * texCoordYMulti;
        data.texCoordX1 = bakedchar.x1 * texCoordXMulti;
        data.texCoordY1 = bakedchar.y0 * texCoordYMulti;
        
        // For the correct char we need to add base to the index.
        const char thechar = index + base;
        charMap[thechar] = data;
    }
}

mono::TextDefinition mono::GenerateVertexDataFromString(const std::string& text, const math::Vector2f& pos, bool center)
{
    mono::TextDefinition textDef;
    textDef.chars = text.length();
    textDef.color[0] = 1.0f;
    textDef.color[1] = 1.0f;
    textDef.color[2] = 1.0f;
    textDef.color[3] = 1.0f;

    
    float xposition = pos.mX;
    float yposition = pos.mY;
    
    if(center)
        xposition -= MeasureString(text).mX / 2.0f;

    for(std::string::const_iterator it = text.begin(), end = text.end(); it != end; ++it)
    {
        const char currentChar = *it;
        
        // Look up char in map.
        const CharDataMap::const_iterator foundChar = charMap.find(currentChar);
        if(foundChar == charMap.end())
            continue;
        
        const CharData& data = foundChar->second;
        
        const float x0 = xposition + data.xoffset;
        const float y0 = yposition - data.yoffset;
        const float x1 = x0 + data.width;
        const float y1 = y0 + data.height;

        // First triangle
        textDef.vertices.push_back(x0);
        textDef.vertices.push_back(y0);
        textDef.texcoords.push_back(data.texCoordX0);
        textDef.texcoords.push_back(data.texCoordY0);
        
        textDef.vertices.push_back(x1);
        textDef.vertices.push_back(y1);
        textDef.texcoords.push_back(data.texCoordX1);
        textDef.texcoords.push_back(data.texCoordY1);
        
        textDef.vertices.push_back(x0);
        textDef.vertices.push_back(y1);
        textDef.texcoords.push_back(data.texCoordX0);
        textDef.texcoords.push_back(data.texCoordY1);
        
        // Second triangle
        textDef.vertices.push_back(x0);
        textDef.vertices.push_back(y0);
        textDef.texcoords.push_back(data.texCoordX0);
        textDef.texcoords.push_back(data.texCoordY0);
        
        textDef.vertices.push_back(x1);
        textDef.vertices.push_back(y0);
        textDef.texcoords.push_back(data.texCoordX1);
        textDef.texcoords.push_back(data.texCoordY0);
        
        textDef.vertices.push_back(x1);
        textDef.vertices.push_back(y1);
        textDef.texcoords.push_back(data.texCoordX1);
        textDef.texcoords.push_back(data.texCoordY1);
        
        // Add x size of char so that the next char is placed a little bit to the right.
        xposition += data.xadvance;
    }
    
    return textDef;
}

void mono::DrawTextFromDefinitions(const TextDefCollection& collection)
{
    if(collection.empty())
        return;
    
    fontTexture->Use();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
    for(TextDefCollection::const_iterator it = collection.begin(), end = collection.end(); it != end; ++it)
    {
        const TextDefinition& def = *it;
                        
        glColor4f(def.color[0], def.color[1], def.color[2], def.color[3]);
        glVertexPointer(2, GL_FLOAT, 0, &def.vertices.front());
        glTexCoordPointer(2, GL_FLOAT, 0, &def.texcoords.front());
        
        // Number of chars in the text, times 3 since each triangle contains 3 vertices,
        // times 2 since each char containts two triangles.
        const int verticesToDraw = def.chars * 3 * 2;
        
        glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


