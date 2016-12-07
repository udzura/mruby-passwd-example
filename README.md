# mruby-passwd-example   [![Build Status](https://travis-ci.org/udzura/mruby-passwd-example.svg?branch=master)](https://travis-ci.org/udzura/mruby-passwd-example)
Passwd class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'udzura/mruby-passwd-example'
end
```
## example
```ruby
p Passwd.hi
#=> "hi!!"
t = Passwd.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
