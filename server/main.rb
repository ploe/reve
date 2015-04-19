#! /usr/bin/ruby

require 'sinatra'

require './Madlib.rb'
require './Dynamo.rb'
require 'json'

get '/:resource/:user/:thing' do
	JSON.generate(params)
end

get '/new' do
	form = Dynamo.new
	form.append({
		'tag' => "INPUT",
		'attributes' => {
			'id' => "drink",
			'name' => "drink",
			'placeholder' => 'name?',
			'type' => 'text',
		} 
	}).select(%w(cider wine other))
	.append({
		'tag' => "BUTTON",
		'content' => "submit"
	}).to_s
end
