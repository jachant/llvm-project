//===- PrintStructInfo.cpp ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt   for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception.
//
//===----------------------------------------------------------------------===//

#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace clang;
using namespace llvm;

namespace {

std::string getAccessString(AccessSpecifier accessSpecifier) {
    switch (accessSpecifier) {
        case AS_public: return "public";
        case AS_protected: return "protected";
        case AS_private: return "private";
        default: return "unknown";
    }
}

std::string getMethodQualifiers(CXXMethodDecl *methodDeclaration) {
    std::string qualifiers;

    if (methodDeclaration->isVirtualAsWritten() && 
        !isa<CXXDestructorDecl>(methodDeclaration)) {
        qualifiers += "|virtual";
    }

    if (methodDeclaration->isPureVirtual()) {
        qualifiers += "|pure";
    }

    if (methodDeclaration->size_overridden_methods() > 0) {
        qualifiers += "|override";
    }

    return qualifiers;
}

class StructPrinter : public RecursiveASTVisitor<StructPrinter> {
public:
    explicit StructPrinter(ASTContext &ctx) : context(ctx) {}

    bool VisitCXXRecordDecl(CXXRecordDecl *declaration) {
        if (!declaration->isThisDeclarationADefinition() || declaration->isImplicit())
            return true;
        if (!context.getSourceManager().isWrittenInMainFile(declaration->getLocation()))
            return true;

        outs() << declaration->getNameAsString();

        if (declaration->getNumBases() > 0) {
            outs() << " -> ";
            bool first = true;
            for (const auto &Base : declaration->bases()) {
                auto *baseType = Base.getType()->getAsCXXRecordDecl();
                if (baseType) {
                    if (!first) {
                        outs() << " -> ";
                    }
                    outs() << baseType->getNameAsString();
                    first = false;
                }
            }
        }
        outs() << "\n";

        outs() << "|_Fields\n";
        for (auto *field : declaration->fields()) {
            outs() << "| |_ " << field->getNameAsString()
                   << " (" << field->getType().getAsString()
                   << "|" << getAccessString(field->getAccess()) << ")\n";
        }

        outs() << "\n|_Methods\n";
        for (auto *method : declaration->methods()) {
            std::string methodAccess = getAccessString(method->getAccess());
            std::string qualifiers = getMethodQualifiers(method);
            std::string returnType = method->getReturnType().getAsString();

            outs() << "| |_ " << method->getNameAsString()
                   << " (" << returnType << "()|" << methodAccess << qualifiers << ")\n";
        }

        outs() << "\n";
        return true;
    }

private:
    ASTContext &context;
};

class PrintStructInfoConsumer : public ASTConsumer {
public:
    explicit PrintStructInfoConsumer(ASTContext &context) : visitor(context) {}
    
    void HandleTranslationUnit(ASTContext &context) override {
        visitor.TraverseDecl(context.getTranslationUnitDecl());
    }

private:
    StructPrinter visitor;
};

class PrintStructInfoAction : public PluginASTAction {
protected:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(
        CompilerInstance &CI, llvm::StringRef InFile) override {
        return std::make_unique<PrintStructInfoConsumer>(CI.getASTContext());
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &args) override {
        if (!args.empty() && args[0] == "-help") {
            PrintHelp(llvm::errs());
        }
        return true;
    }

    void PrintHelp(llvm::raw_ostream& ros) {
        ros << "Usage: -print-struct-info [-help]\n"
            << "Analyzes struct/class type information.\n";
    }
};

} // anonymous namespace

static FrontendPluginRegistry::Add<PrintStructInfoAction>
X("print-struct-info", "Analysis of struct/class type information");