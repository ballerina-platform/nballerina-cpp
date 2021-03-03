#ifndef __BALPACKAGE__H__
#define __BALPACKAGE__H__

#include "interfaces/Translatable.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/MC/StringTableBuilder.h"
#include <map>
#include <string>
#include <vector>

namespace nballerina {

// Forward Declaration
class Function;
class Variable;
class Type;

class Package : public Translatable {
private:
  std::string org;
  std::string name;
  std::string version;
  std::string sourceFileName;
  std::vector<Function *> functions;
  std::map<std::string, Variable *> globalVars;
  std::map<std::string, LLVMValueRef> globalVarRefs;
  std::map<std::string, Function *> functionLookUp;
  llvm::StructType *boxType;
  llvm::StringTableBuilder *strBuilder;
  std::map<std::string, std::vector<LLVMValueRef>> structElementStoreInst;
  std::map<std::string, LLVMValueRef> functionRefs;
  void applyStringOffsetRelocations();

public:
  Package() = default;
  ~Package() = default;

  std::string getOrgName();
  std::string getPackageName();
  std::string getVersion();
  std::string getSrcFileName();
  Variable *getGlobalVariable(std::string name);
  LLVMValueRef getGlobalLLVMVar(std::string globVar);
  LLVMTypeRef getLLVMTypeOfType(Type *typeD);
  llvm::StringTableBuilder *getStrTableBuilder();
  Function *getFunction(std::string name);
  LLVMValueRef getFunctionRef(std::string arrayName);

  void setOrgName(std::string orgName);
  void setPackageName(std::string pkgName);
  void setVersion(std::string verName);
  void setSrcFileName(std::string srcFileName);
  void insertGlobalVar(Variable *var);
  void insertFunction(Function *function);
  void addFunctionRef(std::string arrayName, LLVMValueRef functionRef);
  void addStringOffsetRelocationEntry(std::string eleType,
                                      LLVMValueRef storeInsn);

  void translate(LLVMModuleRef &modRef) final;
};

} // namespace nballerina

#endif //!__BALPACKAGE__H__