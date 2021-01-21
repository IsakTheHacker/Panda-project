#include "gameTasks.h"

namespace task {
	AsyncTask::DoneStatus computePlayerZVelocity(GenericAsyncTask* task, void* data) {
		NodePath panda = *(NodePath*)data;

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
			panda.set_z(player.model.get_pos().get_z() - player.velocity);
		}

		return AsyncTask::DS_cont;		//Continue this task next frame
	}
}