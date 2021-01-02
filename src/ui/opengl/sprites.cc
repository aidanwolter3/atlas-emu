#include "src/ui/opengl/sprites.h"

#include <iostream>
#include <utility>

Sprites::Sprites() {
  // Create the Shaders and Program.
  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/opengl/sprites.vert")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/opengl/sprites.frag")));
  program_ = std::make_unique<Program>(std::move(shaders));

  PrepareTextures();
}

void Sprites::Draw() {
  program_->Use();

  // Assign texture units to the uniform locations.
  glUniform1i(program_->GetUniformLocation("palette"), 0);
  glUniform1i(program_->GetUniformLocation("tiles"), 1);
  glUniform1i(program_->GetUniformLocation("attributes"), 2);
  glUniform1i(program_->GetUniformLocation("palettes"), 3);

  // Bind the textures to the texture units.
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, attributes_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);

  program_->Draw();
}

void Sprites::SetPalettes(std::vector<uint8_t>& palettes) {
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, palettes.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, palettes.data());
}

void Sprites::SetSprites(std::vector<Sprite>& sprites) {
  if (sprites.size() != 64) {
    std::cout << "Invalid number of sprites" << std::endl;
    return;
  }

  // Set the attributes.
  std::vector<uint8_t> attributes;
  for (auto& s : sprites) {
    attributes.push_back(s.attribute);
  }
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, attributes_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, attributes.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, attributes.data());

  // Set the tiles.
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  for (int i = 0; i < sprites.size(); ++i) {
    auto& s = sprites[i];
    const int height = (s.tile.size() == 128 ? 16 : 8);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    /*xoffset=*/0,
                    /*yoffset=*/0,
                    /*zoffset=*/i,
                    /*width=*/8,
                    /*height=*/height,
                    /*depth=*/1, GL_RED_INTEGER, GL_UNSIGNED_BYTE,
                    s.tile.data());
  }

  // Set the vertices/elements/texture coords.
  std::vector<float> vertices;
  std::vector<unsigned int> elements;
  std::vector<float> texture_coords;
  for (int i = 0; i < sprites.size(); ++i) {
    auto& s = sprites[i];
    const int height = (s.tile.size() == 128 ? 16 : 8);
    const bool flipx = s.attribute & 0x40;
    const bool flipy = s.attribute & 0x80;

    float unit_x = 2.0 * 8 / 0x100;
    float unit_y = 2.0 * height / 0xF0;
    float top_left_x = ((float)s.x * 2.0 / 0x100) - 1.0;
    float top_left_y = ((float)(0xF0 - s.y) * 2.0 / 0xF0) - 1.0;

    unsigned int start_element = i * 4;
    vertices.insert(vertices.begin(), {
                                          top_left_x,
                                          top_left_y,
                                          top_left_x + unit_x,
                                          top_left_y,
                                          top_left_x + unit_x,
                                          top_left_y - unit_y,
                                          top_left_x,
                                          top_left_y - unit_y,
                                      });
    elements.insert(elements.end(), {
                                        start_element,
                                        start_element + 1,
                                        start_element + 2,
                                        start_element + 2,
                                        start_element + 3,
                                        start_element,
                                    });

    // Flip the texture coordinates according to the attributes.
    float left_x = 0.0;
    float bottom_y = 0.0;
    float right_x = 1.0;
    float top_y = 1.0;
    if (height == 8) {
      top_y = 0.5;
    }
    if (flipy) {
      std::swap(bottom_y, top_y);
    }
    if (flipx) {
      std::swap(left_x, right_x);
    }

    texture_coords.insert(texture_coords.begin(),
                          {
                              left_x, bottom_y, (float)i,   // bottom-left
                              right_x, bottom_y, (float)i,  // bottom-right
                              right_x, top_y, (float)i,     // top-right
                              left_x, top_y, (float)i,      // top-left
                          });
  }

  // Send the vectors to the program.
  program_->SetVertices(vertices);
  program_->SetElements(elements);
  program_->SetTextureCoords(texture_coords);
}

void Sprites::PrepareTextures() {
  // Declare the tiles.
  // Note: Use a size of 8x16, so that it works for both 8x8 and 8x16 mode. In
  // 8x8 mode, the bottom half will not be used.
  glGenTextures(1, &tiles_);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tiles_);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8UI,
               /*width=*/8,
               /*height=*/16,
               /*count=*/64, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);

  // Declare the attributes.
  glGenTextures(1, &attributes_);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, attributes_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Declare the palettes.
  glGenTextures(1, &palettes_);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palettes_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(0);
}
