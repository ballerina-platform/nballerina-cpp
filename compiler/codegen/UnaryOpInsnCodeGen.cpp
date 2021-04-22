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

#include "UnaryOpInsn.h"
#include "Function.h"
#include "NonTerminatorInsnCodeGen.h"
#include "Operand.h"
#include "Package.h"

namespace nballerina {

void NonTerminatorInsnCodeGen::visit(class UnaryOpInsn &obj, llvm::Module &module, llvm::IRBuilder<> &builder) {
    const auto &funcObj = obj.getFunctionRef();
    const auto &lhsOp = obj.getLhsOperand();
    auto *lhsRef = funcObj.getLLVMLocalOrGlobalVar(lhsOp, module);
    auto *rhsOpref = funcObj.createTempVariable(obj.rhsOp, module, builder);

    switch (obj.kind) {
    case INSTRUCTION_KIND_UNARY_NOT: {
        auto *ifReturn = builder.CreateNot(rhsOpref, lhsOp.getName() + "_temp");
        builder.CreateStore(ifReturn, lhsRef);
        break;
    }
    case INSTRUCTION_KIND_UNARY_NEG: {
        auto *ifReturn = builder.CreateNeg(rhsOpref, lhsOp.getName() + "_temp");
        builder.CreateStore(ifReturn, lhsRef);
        break;
    }
    default:
        llvm_unreachable("");
    }
}

} // namespace nballerina