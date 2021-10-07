#include "gameTasks.h"

namespace task {
	AsyncTask::DoneStatus computePlayerZVelocity(GenericAsyncTask* task, void* data) {
		if (!player.flying) {
			if (player.velocity == 0 && !player.onGround) {
				player.velocity = 0.01;
			} else {
				if (player.velocity > 0) {
					if (player.velocity < 1.25) {
						player.velocity = player.velocity * player.velocityModifier;
					}
				} else if (player.velocity < 0) {
					double value = (int)((double)player.model.get_z() * 100 + 0.5);
					value = (double)value / 100;
					double value2 = (int)((player.model.get_z() - player.velocity) * 100 + 0.5);
					value2 = (double)value2 / 100;
					if (value == value2) {
						player.velocity = 0.01;
					} else {
						player.velocity = player.velocity / player.velocityModifier;
					}
				}
			}
			if (player.onGround) {
				player.velocity = 0;
			}
			player.model.set_z(player.model.get_pos().get_z() - player.velocity);
		}

		return AsyncTask::DS_cont;		//Continue this task next frame
	}

	AsyncTask::DoneStatus setPlayerChunkPos(GenericAsyncTask* task, void* data) {
		if (player.model.get_x() < 0) {
			player.chunk_x = (int)(player.model.get_x() - game::chunk::chunksize) / game::chunk::chunksize;
		} else {
			player.chunk_x = (int)player.model.get_x() / game::chunk::chunksize;
		}
		if (player.model.get_y() < 0) {
			player.chunk_y = (int)(player.model.get_y() - game::chunk::chunksize) / game::chunk::chunksize;
		} else {
			player.chunk_y = (int)player.model.get_y() / game::chunk::chunksize;
		}

		return AsyncTask::DS_cont;		//Continue this task next frame
	}

	AsyncTask::DoneStatus generateChunks(GenericAsyncTask* task, void* data) {
		std::cout << "Test\n";
		std::tuple<WindowFramework*, PandaFramework*, PerlinNoise3*> tuple = *(std::tuple<WindowFramework*, PandaFramework*, PerlinNoise3*>*)data;
		bool chunk_exists;

		if (game::chunk::loaded_chunks.find(std::pair<int, int>(player.chunk_x, player.chunk_y)) != game::chunk::loaded_chunks.end()) {
			chunk_exists = true;
		} else {
			chunk_exists = false;
		}

		if (!chunk_exists && !keys["f5"]) {
			game::chunk chunk(player.chunk_x, player.chunk_y);														//Create new chunk
			chunk.generateChunk(std::get<0>(tuple), *std::get<1>(tuple), *std::get<2>(tuple));						//Apply the generateChunk function on the new chunk
			game::chunks.push_back(chunk);																			//Push the chunk to vector game::chunks
			game::chunk::loaded_chunks.insert(std::pair<int, int>(player.chunk_x, player.chunk_y));
			game::chunk::index[std::pair<int, int>(chunk.x, chunk.y)] = game::chunks.size() - 1;
		}
		return AsyncTask::DS_cont;		//Continue this task next frame
	}

	AsyncTask::DoneStatus updateHotbar(GenericAsyncTask* task, void* data) {
		std::vector<NodePath> hotbar = *(std::vector<NodePath>*)data;

		if (mouseInGame) {
			if (handInventoryIndex < 0) {
				handInventoryIndex = 8;
			} else if (handInventoryIndex > 8) {
				handInventoryIndex = 0;
			}
			for (int i = 0; i < 9; i++) {
				if (i == handInventoryIndex) {
					game::setTexture(hotbar[i], gamePath + (std::string)"models/textures/png/hand-inventory-highlighted.png");
				} else if (i != handInventoryIndex) {
					game::setTexture(hotbar[i], gamePath + (std::string)"models/textures/png/hand-inventory-all.png");
				}
			}
		}

		return AsyncTask::DS_cont;
	}
}