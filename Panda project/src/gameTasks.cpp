#include "gameTasks.h"

namespace task {
	AsyncTask::DoneStatus computePlayerZVelocity(GenericAsyncTask* task, void* data) {
		std::pair<game::Player, NodePath>* pair = (std::pair<game::Player, NodePath>*)data;

		if (!player.flying) {
			if (velocity == 0 && !player.onGround) {
				velocity = 0.01;
			} else {
				if (velocity > 0) {
					if (velocity < 1.25) {
						velocity = velocity * velocityModifier;
					}
				} else if (velocity < 0) {
					double value = (int)((double)player.model.get_z() * 100 + 0.5);
					value = (double)value / 100;
					double value2 = (int)((player.model.get_z() - velocity) * 100 + 0.5);
					value2 = (double)value2 / 100;
					if (value == value2) {
						velocity = 0.01;
					} else {
						velocity = velocity / velocityModifier;
					}
				}
			}
			if (player.onGround) {
				velocity = 0;
			}
			player.model.set_z(player.model.get_pos().get_z() - velocity);
			panda.set_z(player.model.get_pos().get_z() - velocity);
		}

		return AsyncTask::DS_cont;		//Continue this task next frame
	}
}