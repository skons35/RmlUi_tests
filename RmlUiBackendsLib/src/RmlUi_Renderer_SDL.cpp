/*
 * This source file is part of RmlUi, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://github.com/mikke89/RmlUi
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 * Copyright (c) 2019 The RmlUi Team, and contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "RmlUi_Renderer_SDL.h"


// VA : before these types are used (template) : SCNu64, SCNd64, PRId64, PRu64
//      need to provide a def => the one in /gg/include/inttypes.h
#define __PRI_64_LENGTH_MODIFIER__ "ll"
#define PRId64 __PRI_64_LENGTH_MODIFIER__ "d"
#define PRIu64 __PRI_64_LENGTH_MODIFIER__ "u"
#define __SCN_64_LENGTH_MODIFIER__ "ll"
#define SCNd64 __SCN_64_LENGTH_MODIFIER__ "d"
#define SCNu64 __SCN_64_LENGTH_MODIFIER__ "u"
// End of VA

#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/FileInterface.h>
#include <RmlUi/Core/Types.h>
#include <SDL.h>
#include <SDL_image.h>

#include<sstream> //VA test

RenderInterface_SDL::RenderInterface_SDL(SDL_Renderer* renderer) : renderer(renderer) {}

void RenderInterface_SDL::BeginFrame()
{
	//VA temp :
	//Rml::Log::Message(Rml::Log::LT_INFO, "BeginFrame() called");
	
	/* 
	SDL_RenderSetViewport(renderer, nullptr);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	*/
	// VA : same as above with added dd rc code checks
	if (0 != SDL_RenderSetViewport(renderer, nullptr))
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_RenderSetViewport(): %s", SDL_GetError());
	if (0 != SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255))
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_SetRenderDrawColor(): %s", SDL_GetError());
	if (0 != SDL_RenderClear(renderer))
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_RenderClear(): %s", SDL_GetError());
	if (0 != SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_SetRenderDrawBlendMode(): %s", SDL_GetError());
	// End of VA checks
}

void RenderInterface_SDL::EndFrame() 
{
	//VA temp :
	//Rml::Log::Message(Rml::Log::LT_INFO, "EndFrame() called");
	
	// VA : try forcing fflushing rendering    <<< should not be done, only for specifit opengl use, and here no benefit
	//if (0 != SDL_RenderFlush(renderer))
	//	Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_RenderFlush(): %s", SDL_GetError());
		  
}

void RenderInterface_SDL::RenderGeometry(Rml::Vertex* vertices, int num_vertices, int* indices, int num_indices, const Rml::TextureHandle texture,
	const Rml::Vector2f& translation)
{

	// VA tempo display debug :
	//Rml::Log::Message(Rml::Log::LT_INFO, "RenderGeometry() called, num vertices:%d, num indices:%d, texture present:%d", num_vertices, num_indices, (NULL != (SDL_Texture*)texture));	// multiple calls per frame !

	//VA test DEBUT:
	Rml::Vertex* u_vertices = new Rml::Vertex[num_indices]; // prepare array of unique vertices array (used once each only) >> same size of indice array
	int* u_indices = new int[num_indices]; // will be an incremental indice array, from 0 to (num_indices-1)
	// parsing indices array, and create a unique entry in vertices for each
	for(int uidx = 0; uidx < num_indices; uidx++)
	{
		u_indices[uidx] = uidx; // simply set vertex indices as current (unique) index
		// point to vertex info
		const int targ_idx = indices[uidx];
		const Rml::Vertex & targ_vert = vertices[targ_idx];
		// clone vertex info in current index location in new vertex array
		u_vertices[uidx].position[0] = targ_vert.position[0];
		u_vertices[uidx].position[1] = targ_vert.position[1];
		u_vertices[uidx].colour.alpha = targ_vert.colour.alpha;
		u_vertices[uidx].colour.red = targ_vert.colour.red;
		u_vertices[uidx].colour.green = targ_vert.colour.green;
		u_vertices[uidx].colour.blue = targ_vert.colour.blue;
		u_vertices[uidx].tex_coord[0] = targ_vert.tex_coord[0];
		u_vertices[uidx].tex_coord[1] = targ_vert.tex_coord[1];
	}

	/*
	// tempo skipping entries wit texture :	
	if (NULL == (SDL_Texture*)texture) 
	{
		{ // original vertices array :
			std::stringstream ss;
			ss <<"vertices: ";
			for(int i=0;i<num_vertices;i++) 
			{
				const Rml::Vertex & v = vertices[i];
				ss << "(" << v.position[0] << "," << v.position[1] << ") ";
			}
			Rml::Log::Message(Rml::Log::LT_INFO, "%s", ss.str().c_str());
		}
		{ // original indices array :
			std::stringstream ss;
			ss <<"indices: ";
			for(int i=0;i<num_indices;i++) 
			{
				ss << "(" << indices[i] << ") ";
			}
			Rml::Log::Message(Rml::Log::LT_INFO, "%s", ss.str().c_str());
		}
		// our unique arrays conten, of length : num_indices 
		//  (content should match above, except one unique vertex entry used once)
		{ // original vertices array :
			std::stringstream ss;
			ss <<"Unique vertices: ";
			for(int i=0;i<num_indices;i++) 
			{
				const Rml::Vertex & v = u_vertices[i];
				ss << "(" << v.position[0] << "," << v.position[1] << ") ";
			}
			Rml::Log::Message(Rml::Log::LT_INFO, "%s", ss.str().c_str());
		}
		{ // original indices array :
			std::stringstream ss;
			ss <<"Unique indices: ";
			for(int i=0;i<num_indices;i++) 
			{
				ss << "(" << u_indices[i] << ") ";
			}
			Rml::Log::Message(Rml::Log::LT_INFO, "%s", ss.str().c_str());
		}
	}
	else Rml::Log::Message(Rml::Log::LT_INFO, "Skipping details of a using-texture rendering call");
	// end of VA test
	*/

	// VA add MIDDLE :
	SDL_FPoint* positions = new SDL_FPoint[num_indices];

	for (int i = 0; i < num_indices; i++)
	{
		positions[i].x = u_vertices[i].position.x + translation.x;
		positions[i].y = u_vertices[i].position.y + translation.y;
	}

	SDL_Texture* sdl_texture = (SDL_Texture*)texture;

	SDL_Vertex*  sdlVertices = new SDL_Vertex[num_indices];
	for (int i = 0; i < num_indices; i++)
	{
		sdlVertices[i].position.x = u_vertices[i].position.x + translation.x;
		sdlVertices[i].position.y = u_vertices[i].position.y + translation.y;
		sdlVertices[i].color.r = u_vertices[i].colour[0];
		sdlVertices[i].color.g = u_vertices[i].colour[1];
		sdlVertices[i].color.b = u_vertices[i].colour[2];
		sdlVertices[i].color.a = u_vertices[i].colour[3];	
	}

	// limited case : no texture managed :
	/*
	int rc = SDL_RenderGeometry(renderer, NULL, (const SDL_Vertex*) &sdlVertices[0], num_indices, u_indices, num_indices); //NULL, 0);
	if (0 != rc)
			Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_RenderGeometry() : rc=%d ! (not supported if rc==-1)", rc);
	*/

	// complete case :
	int rc = SDL_RenderGeometryRaw(renderer, sdl_texture, &positions[0].x, sizeof(SDL_FPoint), (const SDL_Color*)&u_vertices->colour, sizeof(Rml::Vertex),
			&u_vertices->tex_coord.x, sizeof(Rml::Vertex), num_indices, u_indices, num_indices, 4);
		if (0 != rc)
			Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on DL_RenderGeometryRaw() : rc=%d ! (not supported if rc==-1)", rc);

	
	delete[] sdlVertices;
	// end of VA MIDDLE


/*
	SDL_FPoint* positions = new SDL_FPoint[num_vertices];

	for (int i = 0; i < num_vertices; i++)
	{
		positions[i].x = vertices[i].position.x + translation.x;
		positions[i].y = vertices[i].position.y + translation.y;
	}

	SDL_Texture* sdl_texture = (SDL_Texture*)texture;
*/	
	//SDL_RenderGeometryRaw(renderer, sdl_texture, &positions[0].x, sizeof(SDL_FPoint), (const SDL_Color*)&vertices->colour, sizeof(Rml::Vertex),
	//	&vertices->tex_coord.x, sizeof(Rml::Vertex), num_vertices, indices, num_indices, 4);
	
	// VA : below is usual / original call; simply added rc check :
	/*
		int rc = SDL_RenderGeometryRaw(renderer, sdl_texture, &positions[0].x, sizeof(SDL_FPoint), (const SDL_Color*)&vertices->colour, sizeof(Rml::Vertex),
			&vertices->tex_coord.x, sizeof(Rml::Vertex), num_vertices, indices, num_indices, 4);
		if (0 != rc)
			Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on DL_RenderGeometryRaw() : rc=%d ! (not supported if rc==-1)", rc);
	// end of VA add check
	*/
	
	// VA TEST 1 (ok) : 2 simples triangles and vertx color
	/*
	  SDL_Vertex vert[6]; // 2 triangles; 6 vertices
	  // center
	  vert[0].position.x = 400;
	  vert[0].position.y = 150;
	  vert[0].color.r = 255;
	  vert[0].color.g = 0;
	  vert[0].color.b = 0;
	  vert[0].color.a = 255;
	  // left
	  vert[1].position.x = 200;
	  vert[1].position.y = 450;
	  vert[1].color.r = 0;
	  vert[1].color.g = 0;
	  vert[1].color.b = 255;
	  vert[1].color.a = 255;
	  // right 
	  vert[2].position.x = 600;
	  vert[2].position.y = 450;
	  vert[2].color.r = 0;
	  vert[2].color.g = 255;
	  vert[2].color.b = 0;
	  vert[2].color.a = 255;
	  // fake 1
	  vert[3].position.x = 450;
	  vert[3].position.y =100;
	  vert[3].color.r = 0;
	  vert[3].color.g = 200;     
	  vert[3].color.b = 50;
	  vert[3].color.a = 255;
	  // fake 2
	  vert[4].position.x = 140;
	  vert[4].position.y = 400;
	  vert[4].color.r = 60;
	  vert[4].color.g = 100;     
	  vert[4].color.b = 150;
	  vert[4].color.a = 255;
	  // fake 3
	  vert[5].position.x = 550;
	  vert[5].position.y = 50;
	  vert[5].color.r = 220;
	  vert[5].color.g = 20;     
	  vert[5].color.b = 150;
	  vert[5].color.a = 255;
 	  //SDL_RenderGeometry(renderer, NULL, vert, 3, NULL, 0); //ok : a triangle with gradient color
	  int tIndices[6]={0,1,2,3,4,5};
	  int tNumIndices = 6;
	  SDL_RenderGeometry(renderer, NULL, vert, 6, &tIndices[0], tNumIndices); // no SDL texture to use provided
	  //SDL_RenderGeometry(renderer, sdl_texture, vert, 6, &tIndices[0], tNumIndices); //texture to use provided
	*/
	// End of VA test 1
	  
	//  VA test 2 : partial ok : no texture usage, and filter set of input vertices to process only  multiple of 3 vertices, to avoid rc=-1
	/*
	  SDL_Vertex*  sdlVertices = new SDL_Vertex[num_vertices];
	  for (int i = 0; i < num_vertices; i++)
		{
			sdlVertices[i].position.x = vertices[i].position.x + translation.x;
			sdlVertices[i].position.y = vertices[i].position.y + translation.y;
			sdlVertices[i].color.r = vertices[i].colour[0];
			sdlVertices[i].color.g = vertices[i].colour[1];
			sdlVertices[i].color.b = vertices[i].colour[2];
			sdlVertices[i].color.a = vertices[i].colour[3];	
		}
		
	  //if(NULL == sdl_texture)
	  //	  Rml::Log::Message(Rml::Log::LT_WARNING, "SDL texture is null ...");
	  
	  //int rc = SDL_RenderGeometry(renderer, sdl_texture, &sdlVertices[0], num_vertices, indices, num_indices); // returns NOT SUPPORTED in some cases
	  //int rc = SDL_RenderGeometry(renderer, sdl_texture, &sdlVertices[0], num_vertices, NULL, 0);  // returns NOT SUPPORTED in some cases
	  if(
		  //(  (0 == (num_indices % 4) && (NULL != sdl_texture) ) ) 
		  //||
          ( (0 == (num_indices % 3) ) )	  
		) // VA : very ugly/not perfect trick to try filter indices array that reuses vertices idx (giving pair length
	  {
		  int rc = SDL_RenderGeometry(renderer, NULL, (const SDL_Vertex*) &sdlVertices[0], num_vertices, indices, num_indices); //NULL, 0);
		  //int rc = SDL_RenderGeometry(renderer, sdl_texture, (const SDL_Vertex*) &sdlVertices[0], num_vertices, indices, num_indices); //NULL, 0);
		  if (0 != rc)
			Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_RenderGeometry() : rc=%d ! (not supported if rc==-1)", rc);
	   }
	  delete[] sdlVertices;
	// End of VA test 2
    */

	delete[] positions;

	//VA test FIN:
	delete[] u_vertices;
	delete[] u_indices;
	// end of VA test FIN
}

void RenderInterface_SDL::EnableScissorRegion(bool enable)
{
	if (enable)
		SDL_RenderSetClipRect(renderer, &rect_scissor);
	else
		SDL_RenderSetClipRect(renderer, nullptr);

	scissor_region_enabled = enable;
}

void RenderInterface_SDL::SetScissorRegion(int x, int y, int width, int height)
{
	rect_scissor.x = x;
	rect_scissor.y = y;
	rect_scissor.w = width;
	rect_scissor.h = height;

	if (scissor_region_enabled)
		SDL_RenderSetClipRect(renderer, &rect_scissor);
}

bool RenderInterface_SDL::LoadTexture(Rml::TextureHandle& texture_handle, Rml::Vector2i& texture_dimensions, const Rml::String& source)
{
	Rml::FileInterface* file_interface = Rml::GetFileInterface();
	Rml::FileHandle file_handle = file_interface->Open(source);
	if (!file_handle)
		return false;

	file_interface->Seek(file_handle, 0, SEEK_END);
	size_t buffer_size = file_interface->Tell(file_handle);
	file_interface->Seek(file_handle, 0, SEEK_SET);

	char* buffer = new char[buffer_size];
	file_interface->Read(buffer, buffer_size, file_handle);
	file_interface->Close(file_handle);

	const size_t i = source.rfind('.');
	Rml::String extension = (i == Rml::String::npos ? Rml::String() : source.substr(i + 1));

	SDL_Surface* surface = IMG_LoadTyped_RW(SDL_RWFromMem(buffer, int(buffer_size)), 1, extension.c_str());

	bool success = false;

	if (surface)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture)
		{
			texture_handle = (Rml::TextureHandle)texture;
			texture_dimensions = Rml::Vector2i(surface->w, surface->h);
			success = true;
		}

		SDL_FreeSurface(surface);
	}

	delete[] buffer;

	return success;
}

bool RenderInterface_SDL::GenerateTexture(Rml::TextureHandle& texture_handle, const Rml::byte* source, const Rml::Vector2i& source_dimensions)
{
	//VA temp :
	//Rml::Log::Message(Rml::Log::LT_INFO, "generateTexture() called");	
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 rmask = 0xff000000;
	Uint32 gmask = 0x00ff0000;
	Uint32 bmask = 0x0000ff00;
	Uint32 amask = 0x000000ff;
#else
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
#endif

	/*
	SDL_Surface* surface =
		SDL_CreateRGBSurfaceFrom((void*)source, source_dimensions.x, source_dimensions.y, 32, source_dimensions.x * 4, rmask, gmask, bmask, amask);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	*/
	//VA add checks :
	SDL_Surface* surface =
		SDL_CreateRGBSurfaceFrom((void*)source, source_dimensions.x, source_dimensions.y, 32, source_dimensions.x * 4, rmask, gmask, bmask, amask);
	if (NULL == surface)
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_CreateRGBSurfaceFrom(): %s", SDL_GetError());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (NULL == texture)
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_CreateTextureFromSurface(): %s", SDL_GetError());
	if (0 != SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND))
		Rml::Log::Message(Rml::Log::LT_ERROR, "SDL error on SDL_SetTextureBlendMode(): %s", SDL_GetError());
	// end of VA check
	
	SDL_FreeSurface(surface);
	texture_handle = (Rml::TextureHandle)texture;
	return true;
}

void RenderInterface_SDL::ReleaseTexture(Rml::TextureHandle texture_handle)
{
	SDL_DestroyTexture((SDL_Texture*)texture_handle);
}