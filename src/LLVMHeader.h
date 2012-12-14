#ifndef _LLVM_HEADER_
#define _LLVM_HEADER_

#ifdef _WIN32
#undef min
#undef max
#endif

#include "llvm/Type.h"
#include "llvm/Module.h"
#include "llvm/LLVMContext.h"
#include "llvm/Value.h"
#include "llvm/Support/Dwarf.h"
#include "llvm/BasicBlock.h"
//#include "llvm/Constant.h"
#if defined _WIN32 || defined __APPLE__
#include "llvm/Support/IRBuilder.h"
#include "llvm/Analysis/DIBuilder.h"
#include "llvm/Analysis/DebugInfo.h"
#else
#include "llvm/DIBuilder.h"
#include "llvm/IRBuilder.h"
#include "llvm/DebugInfo.h"
#endif
#endif
