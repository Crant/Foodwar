#include "Resource.h"
#include "IwResManager.h"

CDECLARE_SINGLETONS(Resource)

void Resource::Init()
{
	IwGetResManager()->LoadGroup("Resources.group");

	CIwResGroup* group = IwGetResManager()->GetGroupNamed("Resources");

	IwGetResManager()->SetCurrentGroup(group);

	this->zTomato = Iw2DCreateImageResource("tomato");
	this->zCarrot = Iw2DCreateImageResource("carrot");
	this->zCauliflower = Iw2DCreateImageResource("cauliflower");
	this->zPaprika = Iw2DCreateImageResource("paprika");

	//// Load fonts
	this->zFontNormal	= Iw2DCreateFontResource("arial10");
	//this->zFontBold			= Iw2DCreateFontResource("arial10_bold");
	//this->zFontLarge		= Iw2DCreateFontResource("arial14");
}

void Resource::Release()
{
	SAFE_DELETE(this->zTomato);
	SAFE_DELETE(this->zCarrot);
	SAFE_DELETE(this->zCauliflower);
	SAFE_DELETE(this->zPaprika);

	SAFE_DELETE(this->zFontNormal);
}
