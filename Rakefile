# File: Rakefile

require 'bundler/gem_tasks'
require 'rake/extensiontask'

load './keyczar_ruby.gemspec'

Gem::PackageTask.new($keyczar_ruby_gemspec)

Rake::ExtensionTask.new('keyczar_ruby') do |ext|
  ext.source_pattern = "*.{c,cc}"        # monitor file changes to allow simple rebuild.
  ext.gem_spec = $keyczar_ruby_gemspec
end
