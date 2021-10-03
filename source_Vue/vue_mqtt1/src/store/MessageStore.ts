import { Message } from '@/services/MqttClient'
import { computed, reactive } from '@vue/runtime-core'

const maxSize = 5
const messageStore = reactive<Array<Message>>([])

const messages = computed(() => {
  return messageStore.slice().reverse()
})

const addMessage = (message: Message) => {
  messageStore.push(message)
  if (messageStore.length > maxSize) {
    messageStore.shift()
  }
}

// eslint-disable-next-line @typescript-eslint/explicit-module-boundary-types
export const useMessageStore = () => {
  return {
    messages,
    addMessage
  }
}
