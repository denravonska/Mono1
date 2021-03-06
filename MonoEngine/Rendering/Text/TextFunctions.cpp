
#include "TextFunctions.h"
#include "System/File.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/TextureFactory.h"
#include "Math/Vector.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype/stb_truetype.h"

#include <cstring>
#include <unordered_map>


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

    struct FontData
    {
        mono::ITexturePtr texture;
        std::unordered_map<char, CharData> chars;
    };

    std::unordered_map<int, FontData> fonts;
}

void mono::LoadFont(int font_id, const char* font, float size, float scale)
{
    File::FilePtr fontfile = File::OpenBinaryFile(font);
    
    std::vector<byte> fontbuffer;
    File::FileRead(fontfile, fontbuffer);
    
    LoadFontRaw(font_id, fontbuffer.data(), fontbuffer.size(), size, scale);
}

void mono::LoadFontRaw(int font_id, const unsigned char* data_bytes, int data_size, float size, float scale)
{
    constexpr int width = 512;
    constexpr int height = 512;
    constexpr int base = 32;
    constexpr int chars = 224;

    byte bitmap[width * height];
    stbtt_bakedchar chardata[chars];
    
    // The bitmap generated by this function has origo in the upper left corner,
    // thats why a bit down the y texture coordinate is flipped.
    stbtt_BakeFontBitmap(data_bytes, 0, size, bitmap, width, height, base, chars, chardata);

    FontData font_data;
    font_data.texture = mono::CreateTexture(bitmap, width, height, 1);

    constexpr float texCoordXMulti = 1.0f / width;
    constexpr float texCoordYMulti = 1.0f / height;
    
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
        const char thechar = static_cast<char>(index + base);
        font_data.chars[thechar] = data;
    }

    fonts.insert(std::make_pair(font_id, font_data));
}

mono::ITexturePtr mono::GetFontTexture(int font_id)
{
    return fonts.find(font_id)->second.texture;
}

void mono::UnloadFonts()
{
    fonts.clear();
}

mono::TextDefinition mono::GenerateVertexDataFromString(int font_id, const char* text, const math::Vector& pos, bool center)
{
    mono::TextDefinition textDef;
    textDef.chars = static_cast<unsigned int>(std::strlen(text));
    textDef.color = mono::Color::RGBA(1.0f, 1.0f, 1.0f);
    textDef.vertices.reserve(textDef.chars * 12);
    textDef.texcoords.reserve(textDef.chars * 12);
    
    float xposition = pos.x;
    float yposition = pos.y;
    
    if(center)
        xposition -= MeasureString(font_id, text).x / 2.0f;

    const FontData& font_data = fonts.find(font_id)->second;

    while(*text != '\0')
    {
        // Look up char in map.
        const auto foundChar = font_data.chars.find(*text);
        assert(foundChar != font_data.chars.end());

        const CharData& data = foundChar->second;
        
        const float x0 = xposition + data.xoffset;
        const float y0 = yposition - data.yoffset;
        const float x1 = x0 + data.width;
        const float y1 = y0 + data.height;

        // First triangle
        textDef.vertices.emplace_back(x0);
        textDef.vertices.emplace_back(y0);
        textDef.texcoords.emplace_back(data.texCoordX0);
        textDef.texcoords.emplace_back(data.texCoordY0);
        
        textDef.vertices.emplace_back(x1);
        textDef.vertices.emplace_back(y1);
        textDef.texcoords.emplace_back(data.texCoordX1);
        textDef.texcoords.emplace_back(data.texCoordY1);
        
        textDef.vertices.emplace_back(x0);
        textDef.vertices.emplace_back(y1);
        textDef.texcoords.emplace_back(data.texCoordX0);
        textDef.texcoords.emplace_back(data.texCoordY1);
        
        // Second triangle
        textDef.vertices.emplace_back(x0);
        textDef.vertices.emplace_back(y0);
        textDef.texcoords.emplace_back(data.texCoordX0);
        textDef.texcoords.emplace_back(data.texCoordY0);
        
        textDef.vertices.emplace_back(x1);
        textDef.vertices.emplace_back(y0);
        textDef.texcoords.emplace_back(data.texCoordX1);
        textDef.texcoords.emplace_back(data.texCoordY0);
        
        textDef.vertices.emplace_back(x1);
        textDef.vertices.emplace_back(y1);
        textDef.texcoords.emplace_back(data.texCoordX1);
        textDef.texcoords.emplace_back(data.texCoordY1);
        
        // Add x size of char so that the next char is placed a little bit to the right.
        xposition += data.xadvance;

        text++;
    }
    
    return textDef;
}

math::Vector mono::MeasureString(int font_id, const char* text)
{
    const FontData& font_data = fonts.find(font_id)->second;

    math::Vector size;
    
    while(*text != '\0')
    {
        // Look up char in map.
        const auto foundChar = font_data.chars.find(*text);
        assert(foundChar != font_data.chars.end());

        const CharData& data = foundChar->second;
        size.x += data.xadvance;
        size.y = std::max(data.height, size.y);

        text++;
    }
    
    return size;
}

