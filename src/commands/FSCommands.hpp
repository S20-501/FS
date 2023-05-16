//
// Created by evezers on 11/05/23.
//

#ifndef FS_FSCOMMANDS_HPP
#define FS_FSCOMMANDS_HPP

#include "Init.h"
#include "Full.h"
#include "Empty.h"
#include "Enter.h"
#include "Copy.h"
#include "Move.h"
#include "Del.h"
#include "Squeeze.h"
#include "Help.h"

#include "FSCommandFactory.hpp"

typedef FSCommandFactory<std::tuple<
    Init,
    Full,
    Empty,
    Enter,
//        Copy,
//        Move,
    Del
//        Squeeze,
//        Help
> > FSCommands;

#endif //FS_FSCOMMANDS_HPP
