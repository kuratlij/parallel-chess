import selenium
from selenium import webdriver
from selenium.webdriver.support.ui import Select

# Load page

browser = webdriver.Chrome()
browser.get("http://bernd.bplaced.net/fengenerator/fengenerator.html")

pieces = 32
amount = 100

f = open("random_fens_" + str(pieces), "w")

for i in xrange(amount):
    # Find dropdown menu for no of pieces
    dropdown = Select(browser.find_element_by_id("sfig"))
    dropdown.select_by_visible_text(str(pieces))
    
    button = browser.find_element_by_css_selector("input[onclick*='makefen()']")
    button.click()

    # Print fen board
    text = browser.find_element_by_id("fen").get_attribute("value")
    print str(text)
    f.write(text + "\n")
    
f.close()
