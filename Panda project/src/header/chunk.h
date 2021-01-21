#pragma once	//Only include this header file once

//C++ built-in libraries
#include <vector>
#include <set>

//Panda3D libraries
#include <perlinNoise3.h>
#include <pandaFramework.h>

//My libraries
#include "gameObject.h"
#include "gameVars.h"

namespace game {
	class chunk {
	private:
		static PerlinNoise3 perlinNoise;
		static WindowFramework* window;
		static PandaFramework* framework;
	public:
		int x;
		int y;
		std::vector<object> objects;
		NodePath oneMesh;

		//Static member variables
		static std::set<std::pair<int, int>> loaded_chunks;
		static std::map<std::pair<int, int>, int> index;
		static int chunksize;

		chunk(const std::vector<object>& objects, const int& x, const int& y);
		chunk(const int& x, const int& y);
		chunk(std::string path, const int& x, const int& y, WindowFramework*& window, PandaFramework& framework);
		int reset();

		static void setDefaultPerlinNoise3(PerlinNoise3 perlinNoise) {
			chunk::perlinNoise = perlinNoise;
		}
		static PerlinNoise3 getDefaultPerlinNoise3() {
			return chunk::perlinNoise;
		}

		static void setDefaultWindow(WindowFramework*& window) {
			chunk::window = window;
		}
		static WindowFramework* getDefaultWindow() {
			return chunk::window;
		}

		static void setDefaultFramework(PandaFramework& Framework) {
			chunk::framework = framework;
		}
		static PandaFramework* getDefaultFramework() {
			return chunk::framework;
		}

		/// <summary> Saves a specified chunk to it's destination. </summary>
		/// <param name="chunk">- your specifed chunk object</param>
		/// <returns> 0 if successful, nonzero if not! </returns>
		int saveChunk() const;

		int generateChunk(WindowFramework*& window, PandaFramework& framework, const PerlinNoise3& perlinNoise);

		/// <summary> Reads a chunk from the specified path. </summary>
		/// <param name="window">- The window object</param>
		/// <param name="framework">- The framework object</param>
		/// <param name="path">- The path where the chunk file is located</param>
		/// <param name="x">- The chunk's x-pos</param>
		/// <param name="y">- The chunk's y-pos</param>
		/// <returns> 0 if successful, nonzero if not! </returns>
		int readChunk(WindowFramework*& window, PandaFramework& framework, std::string path, int x, int y);
	};

	//Creating vector for chunk class
	extern std::vector<chunk> chunks;
}