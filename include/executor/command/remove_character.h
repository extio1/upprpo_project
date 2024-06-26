#ifndef TEXTEDITOR_INCLUDEEXECUTOR_COMMAND_REMOVECHARACTER_H_
#define TEXTEDITOR_INCLUDEEXECUTOR_COMMAND_REMOVECHARACTER_H_

#include "document/document.h"
#include "document/glyphs/glyph.h"
#include "executor/command.h"

class RemoveCharacter : public ReversibleCommand {
   public:
    explicit RemoveCharacter(std::shared_ptr<IDocument> doc);

    RemoveCharacter(RemoveCharacter&&) = default;
    RemoveCharacter& operator=(RemoveCharacter&&) = default;
    RemoveCharacter(const RemoveCharacter&) = delete;
    RemoveCharacter& operator=(const RemoveCharacter&) = delete;

    void Execute() override;
    void Unexecute() override;

    ~RemoveCharacter() override;

   private:
    std::shared_ptr<IDocument> doc;
    char character;
};

#endif  // TEXTEDITOR_INCLUDEEXECUTOR_COMMAND_REMOVECHARACTER_H_