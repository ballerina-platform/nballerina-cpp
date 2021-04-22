/*
 * Copyright (c) 2021, WSO2 Inc. (http://www.wso2.org) All Rights Reserved.
 *
 * WSO2 Inc. licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "FunctionCodeGen.h"
#include "BasicBlock.h"
#include "BasicBlockCodeGen.h"
#include "CodeGenUtils.h"
#include "Function.h"
#include "FunctionParam.h"

namespace nballerina {

llvm::BasicBlock *FunctionCodeGen::getBasicBlock(const std::string &id) { return basicBlocksMap[id]; }

void FunctionCodeGen::visit(Function &obj, llvm::Module &module, llvm::IRBuilder<> &builder) {

    auto *llvmFunction = module.getFunction(obj.name);
    auto *BbRef = llvm::BasicBlock::Create(module.getContext(), "entry", llvmFunction);
    builder.SetInsertPoint(BbRef);

    // iterate through all local vars.
    size_t paramIndex = 0;
    for (auto const &it : obj.localVars) {
        const auto &locVar = it.second;
        auto *varType = CodeGenUtils::getLLVMTypeOfType(locVar.getType(), module);
        auto *localVarRef = builder.CreateAlloca(varType, nullptr, locVar.getName());
        obj.localVarRefs.insert({locVar.getName(), localVarRef});

        if (locVar.isParamter()) {
            llvm::Argument *parmRef = &(llvmFunction->arg_begin()[paramIndex]);
            auto paramName = obj.requiredParams[paramIndex].getName();
            parmRef->setName(paramName);
            builder.CreateStore(parmRef, localVarRef);
            paramIndex++;
        }
    }

    // iterate through with each basic block in the function and create them
    for (auto &bb : obj.basicBlocks) {
        basicBlocksMap[bb->getId()] = llvm::BasicBlock::Create(module.getContext(), bb->getId(), llvmFunction);
    }

    // creating branch to next basic block.
    if (!obj.basicBlocks.empty()) {
        builder.CreateBr(basicBlocksMap[obj.basicBlocks[0]->getId()]);
    }

    // Now translate the basic blocks (essentially add the instructions in them)
    for (auto &bb : obj.basicBlocks) {
        builder.SetInsertPoint(basicBlocksMap[bb->getId()]);
        BasicBlockCodeGen generator(*this);
        generator.visit(*bb, module, builder);
    }
    CodeGenUtils::injectAbortCall(module, builder, obj.name);
}
} // namespace nballerina