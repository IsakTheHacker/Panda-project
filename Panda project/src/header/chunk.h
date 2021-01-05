#pragma once	//Only include this header file once

//C++ built-in libraries
#include <vector>
#include <set>

//Panda3D libraries
#include <perlinNoise3.h>
#include <pandaFramework.h>

//My libraries
#include "gameObject.h"

namespace game {
	class chunk {
	private:
		static PerlinNoise3 perlinNoise;
	public:
		int x;
		int y;
		std::vector<object> objects;
		static std::set<std::pair<int, int>> index;

		chunk(const std::vector<object>& objects, const int& x, const int& y);
		chunk(const int& x, const int& y);
		int reset();
		static void setDefaultPerlinNoise3(PerlinNoise3 perlinNoise);
		static PerlinNoise3 getDefaultPerlinNoise3();

		/// <summary> Saves a specified chunk to it's destination. </summary>
		/// <param name="chunk">- your specifed chunk object</param>
		/// <returns> 0 if successful, nonzero if not! </returns>
		int saveChunk() const;

		int generateChunk(WindowFramework*& window, PandaFramework& framework, const PerlinNoise3& perlinNoise);
	};

	//Creating vector for chunk class
	extern std::vector<chunk> chunks;

	/// <summary> Reads a chunk from the specified path. </summary>
	/// <param name="window">- The window object</param>
	/// <param name="framework">- The framework object</param>
	/// <param name="path">- The path where the chunk file is located</param>
	/// <param name="x">- The chunk's x-pos</param>
	/// <param name="y">- The chunk's y-pos</param>
	/// <returns> 0 if successful, nonzero if not! </returns>
	int readChunk(WindowFramework*& window, PandaFramework& framework, const std::string& path, int x, int y);
}