#include "backend.h"

BackEnd::BackEnd() {
	fm = new FileManager();
	rm = new RMManager(fm);
	im = new IXManager(fm);
}

