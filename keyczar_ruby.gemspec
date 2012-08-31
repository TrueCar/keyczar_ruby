# -*- encoding: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "keyczar_ruby/version"

$keyczar_ruby_gemspec = Gem::Specification.new do |s|
  s.name = %q{keyczar_ruby}
  s.version = Keyczar::VERSION

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = [%q{TrueCar SF Dev Team}]
  s.date = %q{2012-08-31}
  s.description = %q{ruby wrapper for keyczar crypto library}
  s.email = %q{dev-sf@truecar.com}
  s.files = `git ls-files`.split("\n")
  s.test_files = `git ls-files -- {spec}/*`.split("\n")
#  s.homepage = %q{http://clearbook.truecar.com}
  s.require_paths = [%q{lib}]
  s.rubygems_version = %q{1.8.9}
  s.summary = %q{ruby wrapper for keyczar crypto library}

  s.add_development_dependency(%q<bundler>, ["~> 1.1.2"])
  s.add_development_dependency(%q<rspec>, ["~> 2.7.0"])
  s.add_development_dependency(%q<rr>, [">= 0"])
  s.add_development_dependency(%q<rake>, ["~> 0.9.2"])

  s.add_development_dependency(%q<rake-compiler>, ["~> 0.8.1"])
end

