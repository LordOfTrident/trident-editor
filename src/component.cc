#include "component.hh"

LUIC::Component::Component():
	InFocus(false),
	Visible(true),
	LastInFocus(false),
	Inset(true),
	Flags(0)
{};

void LUIC::Component::SetParent(Component* p_Parent) {
	Parent = p_Parent;

	if (Parent == NULL)
		Wnd.SetPos(PosX, PosY);
	else {
		if (PosX < 1) PosX = 1;
		if (PosY < 1) PosY = 1;

		if (PosX > Parent->GetSizeX() - Wnd.GetSizeX() - 1) PosX = Parent->GetSizeX() - Wnd.GetSizeX() - 1;
		if (PosY > Parent->GetSizeY() - Wnd.GetSizeY() - 1) PosY = Parent->GetSizeY() - Wnd.GetSizeY() - 1;

		Wnd.SetPos(Parent->__gttlpsx() + PosX, Parent->__gttlpsy() + PosY);

		if (Flags & LUIC_FLAGS_COMPONENT_SCALED) {
			SetSize(Parent->GetSizeX() - 2, Parent->GetSizeY() - 2);
			SetPos(1, 1);
		};
	};
};

void LUIC::Component::SetColorscheme(colors p_ColorScheme) {
	ColorScheme = p_ColorScheme;
};

colors LUIC::Component::GetColorscheme() {
	return ColorScheme;
};

void LUIC::Component::SetIOHandle(IOHandle* p_IOH) {
	IOH = p_IOH;
};

void LUIC::Component::SetFlags(flags p_flags) {
	Flags = p_flags;
};

flags LUIC::Component::GetFlags() {
	return Flags;
};

void LUIC::Component::AddChild(Component* p_child) {
	p_child->SetParent(this);
	p_child->SetIOHandle(IOH);

	Children .push_back (p_child);
};

i16 LUIC::Component::GetChild(Component* p_child) {
	for (ui16 i = 0; i < (ui16)Children.size(); ++ i)
		if (Children[i] == p_child)
			return i;

	return -1;
};

void LUIC::Component::RemoveChild(ui16 p_idx) {
	Children.erase(Children.begin() + p_idx);
};

void LUIC::Component::PushBackChild(Component* p_child) {
	RemoveChild(GetChild(p_child));
	AddChild(p_child);
};

void LUIC::Component::SetPos(i16 p_PosX, i16 p_PosY) {
	if (p_PosX < 0)
		p_PosX = 0;

	if (p_PosY < 0)
		p_PosY = 0;

	PosX = p_PosX;
	PosY = p_PosY;

	if (Parent == NULL)
		Wnd.SetPos(PosX, PosY);
	else {
		if (PosX < Inset)
			PosX = Inset;

		if (PosY < Inset)
			PosY = Inset;

		if (PosX > Parent->GetSizeX() - Wnd.GetSizeX() - Inset)
			PosX = Parent->GetSizeX() - Wnd.GetSizeX() - Inset;

		if (PosY > Parent->GetSizeY() - Wnd.GetSizeY() - Inset)
			PosY = Parent->GetSizeY() - Wnd.GetSizeY() - Inset;

		Wnd.SetPos(Parent->__gttlpsx() + PosX, Parent->__gttlpsy() + PosY);
	};

	__fxps();
};

void LUIC::Component::SetInset(bool p_on) {
	Inset = p_on;
};

bool LUIC::Component::GetInset() {
	return Inset;
};

ui16 LUIC::Component::GetPosX() {
	return PosX;
};

ui16 LUIC::Component::GetPosY() {
	return PosY;
};

void LUIC::Component::SetSize(i16 p_SizeX, i16 p_SizeY) {
	if (p_SizeX < 0)
		p_SizeX = 0;

	if (p_SizeY < 0)
		p_SizeY = 0;

	Wnd.SetSize(p_SizeX, p_SizeY);

	for (i16 i = (i16)Children.size() - 1; i >= 0; -- i) {
		Component *child = Children[i];

		if (child->GetFlags() & LUIC_FLAGS_COMPONENT_SCALED) {
			child->SetSize(p_SizeX - 2, p_SizeY - 2);
			child->SetPos(1, 1);
		};
	};
};

ui16 LUIC::Component::GetSizeX() {
	return Wnd.GetSizeX();
};

ui16 LUIC::Component::GetSizeY() {
	return Wnd.GetSizeY();
};

void LUIC::Component::SetFocus(bool p_fcs) {
	InFocus = p_fcs;
};

bool LUIC::Component::IsInFocus() {
	return InFocus;
};

void LUIC::Component::ToTop() {
	if (Parent == NULL)
		IOH->PushBackChild(this);
	else
		Parent->PushBackChild(this);
};

void LUIC::Component::SetVisible(bool p_Visible) {
	Visible = p_Visible;
};

bool LUIC::Component::GetVisible() {
	return Visible;
};

str LUIC::Component::GetType() {
	return Type;
};

bool LUIC::Component::IsLastInFocus() {
	return LastInFocus;
};

LUIC::Component* LUIC::Component::GetParent() {
	return Parent;
};

LUIC::IOHandle* LUIC::Component::GetIOHandle() {
	return IOH;
};

void LUIC::Component::__sttlps(ui16 p_PosX, ui16 p_PosY) {
	Wnd.SetPos(p_PosX, p_PosY);
};

ui16 LUIC::Component::__gttlpsx() {
	return Wnd.GetPosX();
};

ui16 LUIC::Component::__gttlpsy() {
	return Wnd.GetPosY();
};

void LUIC::Component::__fcsprnts() {
	if (Parent != NULL) {
		Parent->PushBackChild(this);
		Parent->__fcsprnts();
	} else {
		IOH->PushBackChild(this);

		InFocus = true;
		LastInFocus = true;
	};
};

void LUIC::Component::__fxps() {
	for (i16 i = (i16)Children.size() - 1; i >= 0; -- i) {
		Children[i]->SetPos(Children[i]->GetPosX(), Children[i]->GetPosY());
		Children[i]->__fxps();
	};
};

LUIC::Window* LUIC::Component::__gwndacs() {
	return &Wnd;
};
