#include "document/glyphs/monoglyph.h"

MonoGlyph::MonoGlyph(GlyphPtr& glyph)
    : Glyph(glyph->GetPosition().x, glyph->GetPosition().y, glyph->GetWidth(),
            glyph->GetHeight()) {
    std::cout << "Monoglyph::Constructor()" << std::endl;
    component = glyph;
}

Glyph::GlyphList MonoGlyph::Select(const Glyph::GlyphPtr& area) {
    return component->Select(area);
}

void MonoGlyph::Add(GlyphPtr glyph) {
    std::cout << "Monoglyph::Add()" << std::endl;
    component->Add(glyph);
}

void MonoGlyph::Insert(GlyphPtr& glyph) {
    std::cout << "Monoglyph::Insert()" << std::endl;
    component->Insert(glyph);
}

void MonoGlyph::Remove(const GlyphPtr& glyph) {
    std::cout << "Monoglyph::Remove()" << std::endl;
    component->Remove(glyph);
}

Glyph::GlyphPtr MonoGlyph::GetFirstGlyph() { return component; }
Glyph::GlyphPtr MonoGlyph::GetLastGlyph() { return component; }

Glyph::GlyphPtr MonoGlyph::GetNextGlyph(GlyphPtr& glyph) { return nullptr; }
Glyph::GlyphPtr MonoGlyph::GetPreviousGlyph(GlyphPtr& glyph) { return nullptr; }

std::shared_ptr<Glyph> MonoGlyph::Clone() const {
    // MonoGlyph* copy =
    //     new MonoGlyph(this->component);
    // return std::make_shared<MonoGlyph>(*copy);
    return nullptr;  //???
}