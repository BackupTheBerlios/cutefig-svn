

PAGES = index status docs userdoc developer download

LAYOUT = layout.tmpl
CFSTYLE = cfstyle.css
MENUFILE = menu.lst

UPLOADTARGET = shell.berlios.de:/home/groups/cutefig/htdocs

PAGESHTML = $(addsuffix .html, $(PAGES))
PAGESTMPL = $(addsuffix .tmpl, $(PAGES))

PAGESUPLOAD = $(addsuffix .upload, $(PAGES)) $(CFSTYLE:css=upload)

.SUFFIXES: .html .tmpl .upload .css

html: .targ_html

upload: .targ_upload

$(PAGESHTML): $(LAYOUT) $(CFSTYLE) $(MENUFILE) mkhtml.pl 

.targ_html: $(PAGESHTML) 
	touch .targ_html

.targ_upload: $(PAGESUPLOAD)
	scp `cat uploadlist` $(UPLOADTARGET)
	rm uploadlist
	touch .targ_upload

.PHONY: clean
clean:
	rm $(PAGESHTML) $(PAGESUPLOAD) .targ_html

.tmpl.html:
	mkhtml.pl $< > $@ 2>/dev/null

.html.upload:
	echo $< >> uploadlist
	touch $@

.css.upload:
	echo $< >> uploadlist
	touch $@