##
## Passwd Test
##

assert("Passwd#uid") do
  t = Passwd.new "root"
  assert_equal(0, t.uid)
end

assert("Passwd#shell") do
  t = Passwd.new "root"
  assert_equal("/bin/bash", t.shell)
end
