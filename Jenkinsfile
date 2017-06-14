#!/usr/bin/env groovy

pipeline {
	agent any

	options {
		skipDefaultCheckout()
	}

	stages {
		stage('Checkout') {
			steps {
				checkout([
					$class: 'GitSCM',
					branches: scm.branches,
					doGenerateSubmoduleConfigurations: false,
					extensions: scm.extensions +
					            [[$class: 'SubmoduleOption',
					              disableSubmodules: false,
					              parentCredentials: false,
					              recursiveSubmodules: true,
					              reference: '',
					              trackingSubmodules: false]] +
					            [[$class: 'CleanBeforeCheckout']],
					userRemoteConfigs: scm.userRemoteConfigs
				])
			}
		}

		stage('Build') {
			steps {
				sh "make -j4 dist"
			}
		}

		stage('Test') {
			steps {
				sh "make unittest"
			}
		}
	}
}