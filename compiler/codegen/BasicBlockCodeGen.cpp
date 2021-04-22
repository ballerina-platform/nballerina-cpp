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

#include "BasicBlockCodeGen.h"
#include "BasicBlock.h"
#include "NonTerminatorInsn.h"
#include "NonTerminatorInsnCodeGen.h"
#include "TerminatorInsn.h"
#include "TerminatorInsnCodeGen.h"

namespace nballerina {

BasicBlockCodeGen::BasicBlockCodeGen(FunctionCodeGen &parentGenerator) : parentGenerator(parentGenerator) {}

void BasicBlockCodeGen::visit(BasicBlock &obj, llvm::Module &module, llvm::IRBuilder<> &builder) {

    for (const auto &instruction : obj.instructions) {
        NonTerminatorInsnCodeGen generator(parentGenerator);
        instruction->accept(generator, module, builder);
    }
    if (obj.terminator != nullptr) {
        TerminatorInsnCodeGen generator(parentGenerator);
        obj.terminator->accept(generator, module, builder);
    }
}
} // namespace nballerina