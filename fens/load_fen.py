import selenium
from selenium import webdriver

# Load page

browser = webdriver.Chrome()
browser.get("http://bernd.bplaced.net/fengenerator/fengenerator.html")

int pieces = 16

f = open("random_fens_" + str(pieces), "w")

for i in xrange(100):
    button = browser.find_element_by_css_selector("input[onclick*='makefen()']")
    button.click()

    # Print fen board
    text = browser.find_element_by_id("fen").get_attribute("value")
    print str(text)
    f.write(text + "\n")
    
f.close()
