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
        container('cbuild') {
          sh 'make install'
        }
      }
    }
  }
}
