#!/bin/bash

for i in resources/translations/*.ts; do
	lrelease $i;
done