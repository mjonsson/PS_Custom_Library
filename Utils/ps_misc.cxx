#include "ps_global.hxx"

bool ps_null_or_empty(const char *str)
{
	if (str == NULL) return true;
	if (strlen(str) == 0) return true;

	return false;
}

tag_t ps_id_of_attr(const char *attrName, const char *className)
{
	tag_t attrId;
	
	itk(POM_attr_id_of_attr(attrName, className, &attrId));

	return attrId;
}

tag_t *ps_id_of_attrs(int numAttrs, const char **attrName, const char **className)
{
	c_ptr<tag_t>	attrIds;

	attrIds.alloc(numAttrs);
	attrIds.set_free(false);

	for (int i = 0; i < numAttrs; i++)
	{
		itk(POM_attr_id_of_attr(attrName[i], className[i], attrIds.get_ptr(i)));
	}


	return attrIds.get();
}
