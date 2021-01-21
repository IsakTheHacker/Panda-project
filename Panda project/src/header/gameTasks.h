#pragma once

//Panda3D libraries
#include <asyncTask.h>
#include <genericAsyncTask.h>
#include <nodePath.h>

//My libraries
#include <gameObject.h>

namespace task {
	AsyncTask::DoneStatus computePlayerZVelocity(GenericAsyncTask* task, void* data);
}