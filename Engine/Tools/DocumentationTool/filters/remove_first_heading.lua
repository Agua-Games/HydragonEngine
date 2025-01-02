-- Remove the first h1 heading from the document body
-- but preserve it for the TOC
local first_header = true

function Header(el)
    if first_header and el.level == 1 then
        first_header = false
        return {}
    end
    return el
end 