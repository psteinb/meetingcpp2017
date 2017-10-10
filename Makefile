all : serve

serve :
	bundle exec jekyll serve

clean :
	bundle exec jekyll clean

build :
	bundle exec jekyll build
