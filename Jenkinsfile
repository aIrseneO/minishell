pipeline {
  agent {
    kubernetes {
      yaml '''
        apiVersion: v1
        kind: Pod
        spec:
          containers:
          - name: cbuilder
            image: airseneo/cbuilder
            tty: true
        '''
    }
  }
  stages {
    stage('Build') {
      steps {
        container('cbuilder') {
          sh 'make install'
        }
      }
    }
    stage('Test') {
      steps {
        container('cbuilder') {
          sh 'make test'
        }
      }
    }
  }
  post {
    success {
      slackSend(channel: 'jenkinsci', message: "Pipeline succeeded - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)")
    }

    failure {
      slackSend(channel: 'jenkinsci', message: "Pipeline failed - ${env.JOB_NAME} ${env.BUILD_NUMBER} (<${env.BUILD_URL}|Open>)")
    }

  }
}
